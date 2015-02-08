// Contains functions and constructors for the Server class
// Needed Header
#include "server.h"
// Functions
void Server::Start(const Config &config,Game_Setup &game_setup,Game &game,std::vector<Player> &player){
    // Check if clean
    if(game.server[0]){
        Shutdown(game,player,game_setup);
    }
    // At least one player is needed for the server at the moment.
    game.id=0;
    if(player.size()==0){
        game_setup.Add_Player(game,player);
        #ifdef DEBUG
        // Set keys
        player[0].keyL=sf::Keyboard::Left;
        player[0].keyR=sf::Keyboard::Right;
        #endif // DEBUG
    }
    game.refresh_players=true;
    // Create Server Threads
    thread_listener = std::thread(&Server::Server_Listener,this,std::cref(config),std::ref(game_setup),std::ref(game),std::ref(player));
    thread_sender = std::thread(&Server::Server_Sender,this,std::cref(config),std::ref(game),std::ref(player));
    game.server_ip=sf::IpAddress::getLocalAddress().toString();
}
//
void Server::Server_Listener(const Config &config,Game_Setup &game_setup,Game &game,std::vector<Player> &player){
    std::cout << "Server listener started!!" << std::endl;
    // Init
    for(int i=0; i<config.max_attempts&&!game.server[1];i++){
        if (listener.listen(config.port) == sf::Socket::Done){
            selector.add(listener);
            std::cout << "Bound!" << std::endl;
            game.server[1]=true;
            break;
        }
        sf::sleep(sf::milliseconds(config.attempt_delay));
    }
    if(!game.server[1]){
        game.server[2]=true;
        std::cout << "Can't bind! Please retry." << std::endl;
    }
    // Main Loop
    while(!game.server[2]){
        // Selector
        if(selector.wait(sf::milliseconds(1000))){
            // Als het de listener is
            if(selector.isReady(listener)&&player.size()<6&&game.mode!=Game::Mode::Play){
                clients.emplace_back(Client_Info());
                if (listener.accept(*clients.back().socket) != sf::Socket::Done){
                    std::cout << "Incoming connection failed.\n";
                    clients.pop_back();
                }
                else{
                    selector.add(*clients.back().socket);
                    std::cout << "Remote client " + clients.back().socket->getRemoteAddress().toString() + " has connected to the server. \n";
                }
            }
            // Loop clients
            bool stop=false; // Stop when disconnection!
            for(unsigned int i=0;i<clients.size()&&!stop;i++){
                if(selector.isReady(*clients[i].socket)){
                    packet.clear();
                    switch(clients[i].socket->receive(packet)){
                        case sf::Socket::Done:
                            // Process Packet
                            Process_Package(config,game_setup,game,player,packet,i);
                            break;
                        case sf::Socket::NotReady:
                            std::cout << "Client not ready?!?" <<std::endl;
                            break;
                        case sf::Socket::Disconnected:
                        case sf::Socket::Error:
                        default:
                            stop=true;
                            std::cout << "Remote client disconnected." << std::endl;
                            Disconnect_Client(game_setup, game, player, i);
                            // Find out if all players left
                            if(player.size()<=1){
                                // Return to setup screen
                                game.Quit(config);
                            }
                            break;
                    }
                }
            } // End Clients
        }     // End Selector
    }
    // Shutdown
    sf::sleep(sf::seconds(1));
    selector.clear();
    clients.clear();
    listener.close();
    game.server[1]=game.server[2]=false;
    game.server[0]=true;
    //
    std::cout << "Server listener ended!" << std::endl;
}
//
void Server::Server_Sender(const Config &config,Game &game,std::vector<Player> &player){
    // Init delay
    sf::sleep(sf::milliseconds(250));
    std::cout << "Server Sender started!!" << std::endl;
    // Main Loop
    while(!game.server[2]){
        // Check if there are packages
        //for(int i=game.packets.size()-1;i>=0;i--){
        for(unsigned int i=0;i<game.packets.size();i++){
            // Remove packet if no clients
            if(clients.size()==0){
                game.mutex.lock();
                game.packets.erase(game.packets.begin()+i);
                game.mutex.unlock();
                i--;
                continue;
            }
            // Check if send id has been set
            else if(game.packets[i].send_id.size()==0){
                continue;
            }
            // Send to all if send_id[0]=-1;
            else if(game.packets[i].send_id[0]==-1){
                game.packets[i].send_id.clear();
                for(unsigned int j=0;j<clients.size();j++){
                    game.packets[i].send_id.push_back(j);
                }
            }
            // Send loop
            for(int j=game.packets[i].send_id.size()-1;j>=0;j--){
                // Check if client still exists
                if(game.packets[i].send_id[j]>=clients.size()){
                    game.packets[i].send_id.erase(game.packets[i].send_id.begin()+j);
                }
                else{
                    switch(clients[game.packets[i].send_id[j]].socket->send(game.packets[i].packet)){
                        case sf::Socket::Done:
                            // Remove Send id
                            game.packets[i].send_id.erase(game.packets[i].send_id.begin()+j);
                            break;
                        case sf::Socket::NotReady:
                            std::cout << "Client not ready: " << clients[game.packets[i].send_id[j]].socket->getRemoteAddress().toString()  << std::endl;
                            break;
                        case sf::Socket::Disconnected:
                        case sf::Socket::Error:
                        default:
                            std::cout << "Failed to send packet:" << clients[game.packets[i].send_id[j]].socket->getRemoteAddress().toString()  << std::endl;
                            break;
                    } // End switch case
                }
            } // End Send id for loop
            // Remove empty packets
            if(game.packets[i].send_id.size()==0){
                game.mutex.lock();
                game.packets.erase(game.packets.begin()+i);
                game.mutex.unlock();
                i--;
            }
        } // End Loop
        sender_pacer.Pace();
    }
    // Shutdown
    std::cout << "Server Sender ended!" << std::endl;
}
// Great to use a variable named new client that can be false and the function name is New_Client too
void Server::New_Client(const Config &config,Game_Setup &game_setup,Game &game,std::vector<Player> &player, const unsigned int &n, const bool &new_client){
    //
    clients[n].version_correct=true;
    // Add new player and synchronize positions between client_info and player vector
    game_setup.Add_Player(game,player);
    clients[n].id.push_back(player.size()-1);
    player.back().id=n;
    player.back().local=false;
    player.back().server=false;
    // Send everything to clients
    Sync_Clients(game,player);
    sf::sleep(sf::milliseconds(250)); // Fixes some errors
    if(new_client){
        Pending pending;
        pending.packet << Packet::ID << (player.size()-1);
        pending.send_id.push_back(n);
        game.mutex.lock();
        game.packets.push_back(pending);
        game.mutex.unlock();
    }
    else{
        Pending pending;
        pending.packet << Packet::Request_Player << (player.size()-1);
        pending.send_id.push_back(n);
        game.mutex.lock();
        game.packets.push_back(pending);
        game.mutex.unlock();
    }
    // Send options
    game.refresh_options=true;
}
//
void Server::Disconnect_Client(Game_Setup &game_setup, Game &game,std::vector<Player> &player, const unsigned int &n){
    // Create Package and remove player
    Pending pending;
    pending.packet << Packet::DCon;
    for(unsigned int j=0;j<clients[n].id.size();j++){
        pending.packet << clients[n].id[j];
        game_setup.Remove_Player(game,player,clients[n].id[j]);
    }
    game.refresh_players=true;
    // Remove out of list
    selector.remove(*clients[n].socket);
    clients.erase(clients.begin()+n);
    // Update id in player
    for(unsigned int j=1;j<player.size();j++){
        if(!player[j].local&&player[j].id>n){
            player[j].id--;
        }
    }
    // Pend Packed
    pending.send_id.push_back(-1);
    game.mutex.lock();
    game.packets.push_back(pending);
    game.mutex.unlock();
}
// n is the place in the client vector
void Server::Process_Package(const Config &config,Game_Setup &game_setup,Game &game,std::vector<Player> &player,sf::Packet &packet, const unsigned int &n){
    Packet type;
    packet >> type;
    //
    if(!clients[n].version_correct){
        if(type==Packet::Version){
            std::string version;
            packet >> version;
            if(version==config.version){
                std::cout << "Client version correct!" << std::endl;
                New_Client(config,game_setup,game,player,n,true);
            }
            else{
                std::cout << "Incorrect client version!" << std::endl;
                Pending pending;
                pending.packet << Packet::Version << false;
                pending.send_id.push_back(n);
                game.mutex.lock();
                game.packets.push_back(pending);
                game.mutex.unlock();
                sf::sleep(sf::seconds(0.5));
                Disconnect_Client(game_setup,game,player,n);
            }
        }
        else{
            // Force disconnect
            Disconnect_Client(game_setup,game,player,n);
        }
    }
    //
    else if(type==Packet::Name){
        int id;
        packet >> id;
        // Data Check
        if(!clients[n].Check_ID(id)){
            return;
        }
        // Update
        packet >> player[id].name;
        // Resend Package
        if(clients.size()>1){
            Pending pending;
            pending.packet << Packet::Name << id << player[id].name;
            for(unsigned int j=0;j<clients.size();j++){
                // For loop here too?
                if(j!=clients[j].id[0]){
                    pending.send_id.push_back(j);
                }
            }
            game.mutex.lock();
            game.packets.push_back(pending);
            game.mutex.unlock();
        }
        game.refresh_players=true;
    }
    else if(type==Packet::Ready){
        packet >> clients[n].ready;
        // Resend Package
        if(clients.size()>1){
            Pending pending;
            pending.packet << Packet::Ready;
            for(unsigned int i=0;i<clients[n].id.size();i++){
                pending.packet << clients[n].id[i] << clients[n].ready;
            }
            // Only send to other clients
            for(unsigned int j=0;j<clients.size();j++){
                if(j!=n){
                    pending.send_id.push_back(j);
                }
            }
            game.mutex.lock();
            game.packets.push_back(pending);
            game.mutex.unlock();
        }
    }
    else if(type==Packet::KeyL){
        int id;
        packet >> id;
        // Data Check
        if(!clients[n].Check_ID(id)){
            return;
        }
        //
        packet >> player[id].left;
    }
    else if(type==Packet::KeyR){
        int id;
        packet >> id;
        // Data Check
        if(!clients[n].Check_ID(id)){
            return;
        }
        //
        packet >> player[id].right;
    }
    else if(type==Packet::Lag){
        int id;
        packet >> id;
        // Data Check
        if(!clients[n].Check_ID(id)){
            return;
        }
        //
        std::cout << player[id].name.toAnsiString() << "(" << id << ") " << " lags";
        game.Pause(true);
    }
    // This is almost the same as is done in New_Client
    else if(type==Packet::Request_Player){
        // Check if there are free players
        if(player.size()==config.max_players||!game.multiple_players_enabled){
            // If not then mention it to the client
            Pending pending;
            pending.packet << Packet::Request_Player << -1;
            pending.send_id.push_back(n);
            game.mutex.lock();
            game.packets.push_back(pending);
            game.mutex.unlock();
            return;
        }
        // Create new player
        New_Client(config,game_setup,game,player,n,false);
    }
    else if(type==Packet::Remove_Player){
        unsigned int id;
        packet >> id;
        // Data Check
        if(!clients[n].Check_ID(id)){
            return;
        }
        // Remove
        game_setup.Remove_Player(game,player,id);
        game.refresh_players=true;
        //
        // Remove from clients
        for(unsigned int j=0;j<clients[n].id.size();j++){
            if(clients[n].id[j]==id){
                clients[n].id.erase(clients[n].id.begin()+j);
                break;
            }
        }
        // Update player id's
        Update_Player_id(player,id);
        // Send to other clients
        Pending pending;
        pending.packet << Packet::DCon << id;
        pending.send_id.push_back(-1);
        game.mutex.lock();
        game.packets.push_back(pending);
        game.mutex.unlock();
    }
}
//
void Server::Shutdown(Game &game){
    game.server[2]=true;
    if(thread_listener.joinable()){
        thread_listener.join();
    }
    if(thread_sender.joinable()){
        thread_sender.join();
    }
    game.server[0]=game.server[2]=false;
}
void Server::Shutdown(Game &game,std::vector<Player> &player, Game_Setup &game_setup){
    Shutdown(game);
    // Remove players that are not local
    for(unsigned int i=0;i<player.size();i++){
        if(!player[i].local){
            game_setup.Remove_Player(game,player,i);
            i--;
        }
    }
}
//
void Server::Sync_Clients(Game &game,const std::vector<Player> &player){
    // All player information
    Pending pending;
    pending.packet << Packet::Sync;
    for(unsigned int i=0;i<player.size();i++){
        pending.packet << i << player[i].name << player[i].color << player[i].server << player[i].ready;
    }
    pending.send_id.push_back(-1);
    game.mutex.lock();
    game.packets.push_back(pending);
    game.mutex.unlock();
}
// n denotes the player that will be removed
void Server::Update_Player_id(const std::vector<Player> &player,const unsigned int &n){
    for(unsigned int i=0;i<clients.size();i++){
        for(unsigned int j=0;j<clients[i].id.size();j++){
            if(clients[i].id[j]>n){
                clients[i].id[j]--;
            }
        }
    }
}
//
