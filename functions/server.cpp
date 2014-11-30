// Contains server functions
#include "../curve.h"
//
void Start_Server(const Config &config,Game &game,Player player[],Server &server){
    game.server[0]=false;
    game.id=0;
    game.players=1;
    Initialize_Game_Setup_MP(config,game,player);
    player[0].enabled=true;
    player[0].ready=true;
    player[0].place=0;
    // Create Server Threads
    game.threads.emplace_back(Server_Listener,std::cref(config),std::ref(game),player,std::ref(server));
    game.threads.emplace_back(Server_Sender,std::cref(config),std::ref(game),player,std::ref(server));
    // Do some more things
    game.server_ip=sf::IpAddress::getLocalAddress().toString();
    // Server bootup complete
    game.server[1]=true;
}
//
void Server_Listener(const Config &config,Game &game,Player player[],Server &server){
    std::cout << "Server listener started!!" << std::endl;
    // Init
    for(int i=0; i<config.max_attempts&&!game.connected;i++){
        if (server.listener.listen(config.port) == sf::Socket::Done){
            server.selector.add(server.listener);
            std::cout << "Bound!" << std::endl;
            game.connected=true;
            break;
        }
        sf::sleep(sf::milliseconds(config.attempt_delay));
    }
    if(!game.connected){
        game.server[2]=true;
        std::cout << "Can't bind! Shutting down server." << std::endl;
    }
    // Main Loop
    while(!game.server[2]){
        // Selector
        if(server.selector.wait(sf::milliseconds(1000))){
            // Als het de listener is
            if(server.selector.isReady(server.listener)&&game.players<MAX_PLAYERS&&game.mode!=Game::Mode::Play_MP){
                server.clients.emplace_back(Client());
                if (server.listener.accept(*server.clients.back().socket) != sf::Socket::Done){
                    std::cout << "Incoming connection failed.\n";
                    server.clients.pop_back();
                }
                else{
                    server.selector.add(*server.clients.back().socket);
                    //(*server.clients.back().socket).setBlocking(false);
                    server.clients.back().ip=server.clients.back().socket->getRemoteAddress();
                    server.client_count++;
                    game.players=server.client_count+1;
                    std::cout << "Remote client " + server.clients.back().ip.toString() + " has connected to the server.\n";
                    server.clients.back().connected=true;
                    Client_Init_Packages(config,game,player,server);
                }
            }
            // Loop clients
            bool stop=false; // Stop when disconnection!
            for(int i=0;i<server.client_count&&!stop;i++){
                if(server.selector.isReady(*server.clients[i].socket)){
                    server.packet.clear();
                    switch(server.clients[i].socket->receive(server.packet)){
                        case sf::Socket::Done:
                            // Process Packet
                            Server_Process_Package(config,game,player,server,server.packet);
                            break;
                        case sf::Socket::NotReady:
                            std::cout << "Client not ready?!?" <<std::endl;
                            break;
                        case sf::Socket::Disconnected:
                        case sf::Socket::Error:
                        default:
                            stop=true;
                            std::cout << "Remote client disconnected." << std::endl;
                            server.client_count--;
                            game.players=server.client_count+1;
                            player[server.clients[i].id].enabled=false;
                            // Create Packages
                            Pending pending;
                            pending.packet << Packet::DCon << server.clients[i].id;
                            // Remove out of list
                            server.selector.remove(*server.clients[i].socket);
                            server.clients.erase(server.clients.begin()+i);
                            // Pend Packed
                            game.mutex.lock();
                            game.pending.push_back(pending);
                            game.mutex.unlock();
                            break;
                    }
                }
            } // End Clients
        }     // End Selector
    }
    // Shutdown
    sf::sleep(sf::milliseconds(1000));
    server.selector.clear();
    server.clients.clear();
    server.listener.close();
    game.connected=false;
    game.server[1]=game.server[2]=false;
    //
    std::cout << "Server listener ended!" << std::endl;
}
//}
//
void Server_Sender(const Config &config,Game &game,Player player[],Server &server){
    // Init delay
    sf::sleep(sf::milliseconds(250));
    std::cout << "Server Sender started!!" << std::endl;
    // Main Loop
    while(!game.server[2]){
        // Check if there are packages
        //for(int i=game.pending.size()-1;i>=0;i--){
        for(unsigned int i=0;i<game.pending.size();i++){
            // Remove packet if no clients
            if(server.client_count==0){
                game.mutex.lock();
                game.pending.erase(game.pending.begin()+i);
                game.mutex.unlock();
                i--;
                continue;
            }
            // Check if send id has been set
            else if(game.pending[i].send_id.size()==0){
                continue;
            }
            // Send to all if send_id[0]=-1;
            else if(game.pending[i].send_id[0]==-1){
                game.pending[i].send_id.clear();
                for(int j=0;j<server.client_count;j++){
                    game.pending[i].send_id.push_back(j);
                }
            }
            // Send loop
            for(int j=game.pending[i].send_id.size()-1;j>=0;j--){
                switch(server.clients[game.pending[i].send_id[j]].socket->send(game.pending[i].packet)){
                    case sf::Socket::Done:
                        // Remove Send id
                        game.pending[i].send_id.erase(game.pending[i].send_id.begin()+j);
                        break;
                    case sf::Socket::NotReady:
                        std::cout << "Client not ready: " << server.clients[game.pending[i].send_id[j]].ip.toString()  << std::endl;
                        break;
                    case sf::Socket::Disconnected:
                    case sf::Socket::Error:
                    default:
                        std::cout << "Failed to send packet:" << server.clients[game.pending[i].send_id[j]].ip.toString()  << std::endl;
                        break;
                } // End switch case
            } // End Send id for loop
            // Remove empty packets
            if(game.pending[i].send_id.size()==0){
                game.mutex.lock();
                game.pending.erase(game.pending.begin()+i);
                game.mutex.unlock();
                i--;
            }
        } // End Loop
        //sf::sleep(sf::milliseconds(5));
    }
    // Shutdown
    std::cout << "Server Sender ended!" << std::endl;
}
//
void Client_Init_Packages(const Config &config,Game &game,Player player[],Server &server){
    // Give Client an id
    int id=-1;
    for(int i=1;i<MAX_PLAYERS;i++){
        if(!player[i].enabled){
            //
            id=i;
            player[i].enabled=true;
            player[i].id=server.clients.size()-1;
            server.clients.back().id=i;
            // Setup Package
            Pending pending;
            pending.packet << Packet::ID << i;
            pending.send_id.push_back(server.clients.size()-1);
            game.mutex.lock();
            game.pending.push_back(pending);
            game.mutex.unlock();
            break;
        }
    }
    // If id is found :)
    if(id!=-1){
        // Send SYNC packets
        for(int i=0;i<MAX_PLAYERS;i++){
            Pending pending;
            pending.packet << Packet::Sync << i << player[i].name << player[i].enabled << player[i].ready;
            if(id==i){pending.send_id.push_back(-1);}
            else{pending.send_id.push_back(server.clients.size()-1);}
            game.mutex.lock();
            game.pending.push_back(pending);
            game.mutex.unlock();
        }
        //
        Config_Package(game);
    }
    // Do something if none is found
    else{
        // Report!
        std::cout << "No free id found! Please restart server!" << std::endl;
        // Remove out of list
        int i = server.clients.size()-1;
        server.selector.remove(*server.clients[i].socket);
        server.clients.erase(server.clients.begin()+i);
    }
}
//
void Server_Sync_Clients(const Config &config,Game &game,Player player[]){
    for(int i=0;i<MAX_PLAYERS;i++){
        Pending pending;
        pending.packet << Packet::Sync << i << player[i].name << player[i].enabled << player[i].ready;
        pending.send_id.push_back(-1);
        game.mutex.lock();
        game.pending.push_back(pending);
        game.mutex.unlock();
    }
}
//
void Server_Start_Game(const Config &config,Game &game,Player player[]){
    // Check keys
    if(player[game.id].keyL!=sf::Keyboard::Unknown&&player[game.id].keyR!=sf::Keyboard::Unknown&&game.players>1&&game.server[1]){
        // Loop Players
        bool NReady=false;
        player[0].points=0;
        for(int i=1;i<MAX_PLAYERS&&!NReady;i++){
            if(player[i].enabled&&!player[i].ready){
                NReady=true;
                break;
            }
            else{
                player[i].points=0;
                player[i].left=player[i].right=false;
                player[i].place=i;
                player[i].ready=false;
            }
        }
        // If everyone ready start game!
        if(!NReady){
            std::cout << "Start Game" << std::endl;
            // Send Start Packages
            Pending pending;
            pending.packet << Packet::StartGame;
            pending.send_id.push_back(-1);
            game.mutex.lock();
            game.pending.push_back(pending);
            game.mutex.unlock();
            // Set the seed
            sf::Time time=game.randclock.getElapsedTime();
            srand (time.asSeconds()+time.asMilliseconds());
            // Initialize game and round
            game.round=0;
            sf::sleep(sf::milliseconds(20));
            Initialize_New_Round_Server(config,game,player);
            game.update_thread[0]=true;
            game.mode=Game::Mode::Play_MP;
        }
    }
}
//
void Config_Package(Game &game){
    // Send
    Pending pending;
    pending.packet << Packet::Config << game.maxpoints << game.powerup_enabled;
    pending.send_id.push_back(-1);
    game.mutex.lock();
    game.pending.push_back(pending);
    game.mutex.unlock();
}
//
void Server_Process_Package(const Config &config,Game &game,Player player[],Server &server,sf::Packet &packet){
    Packet type;
    packet >> type;
    //
    if(type==Packet::Name){
        int id;
        packet >> id;
        // Update
        packet >> player[id].name;
        // Resend Package
        if(server.client_count>1){
            Pending pending;
            pending.packet << Packet::Name << id << player[id].name;
            for(int j=0;j<server.client_count;j++){
                if(j!=server.clients[j].id){
                    pending.send_id.push_back(j);
                }
            }
            game.mutex.lock();
            game.pending.push_back(pending);
            game.mutex.unlock();
        }
    }
    else if(type==Packet::Ready){
        int id;
        packet >> id;
        packet >> player[id].ready;
        // Resend Package
        if(server.client_count>1){
            Pending pending;
            pending.packet << Packet::Ready << id << player[id].ready;
            for(int j=0;j<server.client_count;j++){
                if(j!=server.clients[j].id){
                    pending.send_id.push_back(j);
                }
            }
            game.mutex.lock();
            game.pending.push_back(pending);
            game.mutex.unlock();
        }
    }
    else if(type==Packet::KeyL){
        int id;
        packet >> id;
        packet >> player[id].left;
    }
    else if(type==Packet::KeyR){
        int id;
        packet >> id;
        packet >> player[id].right;
    }
    else if(type==Packet::Lag){
        int id;
        packet >> id;
        std::cout << id << " lags";
        Pause_Game(config,game,true);
    }
}
