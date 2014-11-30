// Contains functions and constructors for the Server class
// Needed Header
#include "server.h"
// Functions
void Server::Start_Server(const Config &config,Game &game,Player player[]){
    game.server[0]=false;
    // Set player for server
    game.id=0;
    game.players=1;
    player[0].enabled=true;
    player[0].ready=true;
    player[0].place=0;
    // Go to Game Setup screen
    Initialize_Game_Setup_MP(config,game,player);
    // Create Server Threads
    thread_listener = std::thread(&Server::Server_Listener,this,std::cref(config),std::ref(game),player);
    thread_sender = std::thread(&Server::Server_Sender,this,std::cref(config),std::ref(game),player);
    // Do some more things
    game.server_ip=sf::IpAddress::getLocalAddress().toString();
    // Server bootup complete
    game.server[1]=true;
}
//
void Server::Server_Listener(const Config &config,Game &game,Player player[]){
    std::cout << "Server listener started!!" << std::endl;
    // Init
    for(int i=0; i<config.max_attempts&&!game.connected;i++){
        if (listener.listen(config.port) == sf::Socket::Done){
            selector.add(listener);
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
        if(selector.wait(sf::milliseconds(1000))){
            // Als het de listener is
            if(selector.isReady(listener)&&game.players<MAX_PLAYERS&&game.mode!=Game::Mode::Play_MP){
                clients.emplace_back(Client());
                if (listener.accept(*clients.back().socket) != sf::Socket::Done){
                    std::cout << "Incoming connection failed.\n";
                    clients.pop_back();
                }
                else{
                    selector.add(*clients.back().socket);
                    //(*clients.back().socket).setBlocking(false);
                    clients.back().ip=clients.back().socket->getRemoteAddress();
                    client_count++;
                    game.players=client_count+1;
                    std::cout << "Remote client " + clients.back().ip.toString() + " has connected to the \n";
                    clients.back().connected=true;
                    Client_Init_Packages(config,game,player);
                }
            }
            // Loop clients
            bool stop=false; // Stop when disconnection!
            for(int i=0;i<client_count&&!stop;i++){
                if(selector.isReady(*clients[i].socket)){
                    packet.clear();
                    switch(clients[i].socket->receive(packet)){
                        case sf::Socket::Done:
                            // Process Packet
                            Process_Package(config,game,player,packet);
                            break;
                        case sf::Socket::NotReady:
                            std::cout << "Client not ready?!?" <<std::endl;
                            break;
                        case sf::Socket::Disconnected:
                        case sf::Socket::Error:
                        default:
                            stop=true;
                            std::cout << "Remote client disconnected." << std::endl;
                            client_count--;
                            game.players=client_count+1;
                            player[clients[i].id].enabled=false;
                            // Create Packages
                            Pending pending;
                            pending.packet << Packet::DCon << clients[i].id;
                            // Remove out of list
                            selector.remove(*clients[i].socket);
                            clients.erase(clients.begin()+i);
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
    selector.clear();
    clients.clear();
    listener.close();
    game.connected=false;
    game.server[1]=game.server[2]=false;
    //
    std::cout << "Server listener ended!" << std::endl;
}
//
void Server::Server_Sender(const Config &config,Game &game,Player player[]){
    // Init delay
    sf::sleep(sf::milliseconds(250));
    std::cout << "Server Sender started!!" << std::endl;
    // Main Loop
    while(!game.server[2]){
        // Check if there are packages
        //for(int i=game.pending.size()-1;i>=0;i--){
        for(unsigned int i=0;i<game.pending.size();i++){
            // Remove packet if no clients
            if(client_count==0){
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
                for(int j=0;j<client_count;j++){
                    game.pending[i].send_id.push_back(j);
                }
            }
            // Send loop
            for(int j=game.pending[i].send_id.size()-1;j>=0;j--){
                switch(clients[game.pending[i].send_id[j]].socket->send(game.pending[i].packet)){
                    case sf::Socket::Done:
                        // Remove Send id
                        game.pending[i].send_id.erase(game.pending[i].send_id.begin()+j);
                        break;
                    case sf::Socket::NotReady:
                        std::cout << "Client not ready: " << clients[game.pending[i].send_id[j]].ip.toString()  << std::endl;
                        break;
                    case sf::Socket::Disconnected:
                    case sf::Socket::Error:
                    default:
                        std::cout << "Failed to send packet:" << clients[game.pending[i].send_id[j]].ip.toString()  << std::endl;
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
void Server::Client_Init_Packages(const Config &config,Game &game,Player player[]){
    // Give Client an id
    int id=-1;
    for(int i=1;i<MAX_PLAYERS;i++){
        if(!player[i].enabled){
            //
            id=i;
            player[i].enabled=true;
            player[i].id=clients.size()-1;
            clients.back().id=i;
            // Setup Package
            Pending pending;
            pending.packet << Packet::ID << i;
            pending.send_id.push_back(clients.size()-1);
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
            else{pending.send_id.push_back(clients.size()-1);}
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
        int i = clients.size()-1;
        selector.remove(*clients[i].socket);
        clients.erase(clients.begin()+i);
    }
}
//
void Server::Process_Package(const Config &config,Game &game,Player player[],sf::Packet &packet){
    Packet type;
    packet >> type;
    //
    if(type==Packet::Name){
        int id;
        packet >> id;
        // Update
        packet >> player[id].name;
        // Resend Package
        if(client_count>1){
            Pending pending;
            pending.packet << Packet::Name << id << player[id].name;
            for(int j=0;j<client_count;j++){
                if(j!=clients[j].id){
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
        if(client_count>1){
            Pending pending;
            pending.packet << Packet::Ready << id << player[id].ready;
            for(int j=0;j<client_count;j++){
                if(j!=clients[j].id){
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
//
//
void Server::Shutdown(){
    thread_listener.join();
    thread_sender.join();
}
