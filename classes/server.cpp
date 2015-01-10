// Contains functions and constructors for the Server class
// Needed Header
#include "server.h"
// Functions
void Server::Start(const Config &config,Game_Setup &game_setup,Game &game,std::vector<Player> &player){
    game.server[0]=false;
    // Set player for server
    game.id=0;
    //
    if(player.size()>1){
        player.clear();
        for(int i=0;i<6;i++){
            game_setup.color_used[i]=false;
        }
    }
    if(player.size()==0){
        game_setup.Add_Player(config,game,player);
    }
    // Create Server Threads
    thread_listener = std::thread(&Server::Server_Listener,this,std::cref(config),std::ref(game_setup),std::ref(game),std::ref(player));
    thread_sender = std::thread(&Server::Server_Sender,this,std::cref(config),std::ref(game),std::ref(player));
    // Do some more things
    game.server_ip=sf::IpAddress::getLocalAddress().toString();
    // Server bootup complete
    game.server[1]=true;
}
//
void Server::Server_Listener(const Config &config,Game_Setup &game_setup,Game &game,std::vector<Player> &player){
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
        std::cout << "Can't bind! Please restart program." << std::endl;
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
                    clients.back().ip=clients.back().socket->getRemoteAddress();
                    std::cout << "Remote client " + clients.back().ip.toString() + " has connected to the server. \n";
                    clients.back().connected=true;
                    New_Client(config,game_setup,game,player);
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
                            Process_Package(config,game,player,packet,i);
                            break;
                        case sf::Socket::NotReady:
                            std::cout << "Client not ready?!?" <<std::endl;
                            break;
                        case sf::Socket::Disconnected:
                        case sf::Socket::Error:
                        default:
                            stop=true;
                            std::cout << "Remote client disconnected." << std::endl;
                            // Create Packages
                            Pending pending;
                            pending.packet << Packet::DCon << clients[i].id;
                            pending.send_id.push_back(-1);
                            // Remove from vectors
                            game_setup.Remove_Player(config,game,player,clients[i].id);
                            game.refresh_players=true;
                            // Remove out of list
                            selector.remove(*clients[i].socket);
                            clients.erase(clients.begin()+i);
                            // Pend Packed
                            game.mutex.lock();
                            game.packets.push_back(pending);
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
                switch(clients[game.packets[i].send_id[j]].socket->send(game.packets[i].packet)){
                    case sf::Socket::Done:
                        // Remove Send id
                        game.packets[i].send_id.erase(game.packets[i].send_id.begin()+j);
                        break;
                    case sf::Socket::NotReady:
                        std::cout << "Client not ready: " << clients[game.packets[i].send_id[j]].ip.toString()  << std::endl;
                        break;
                    case sf::Socket::Disconnected:
                    case sf::Socket::Error:
                    default:
                        std::cout << "Failed to send packet:" << clients[game.packets[i].send_id[j]].ip.toString()  << std::endl;
                        break;
                } // End switch case
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
//
void Server::New_Client(const Config &config,Game_Setup &game_setup,Game &game,std::vector<Player> &player){
    // Add new player and synchronize positions between client_info and player vector
    game_setup.Add_Player(config,game,player);
    clients.back().id=player.size()-1;
    player.back().id=clients.size()-1;
    std::cout << clients.back().id << ";" << player.back().id << std::endl;
    player.back().local=false;
    player.back().server=false;
    // Send everything to client
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
    sf::sleep(sf::milliseconds(250)); // Fixes some errors
    // Player given to client
    pending.packet.clear();
    pending.send_id.clear();
    pending.packet << Packet::ID << (player.size()-1);
    pending.send_id.push_back(clients.size()-1);
    game.mutex.lock();
    game.packets.push_back(pending);
    game.mutex.unlock();
}
//
void Server::Process_Package(const Config &config,Game &game,std::vector<Player> &player,sf::Packet &packet, const unsigned int &n){
    Packet type;
    packet >> type;
    //
    if(type==Packet::Name){
        int id;
        packet >> id;
        // Update
        packet >> player[id].name;
        // Resend Package
        if(clients.size()>1){
            Pending pending;
            pending.packet << Packet::Name << id << player[id].name;
            for(unsigned int j=0;j<clients.size();j++){
                if(j!=clients[j].id){
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
        bool ready;
        packet >> ready;
        // Do the things
        for(unsigned int i=0;i<player.size();i++){
            if(clients[n].id==i&&player[i].id==n){
                player[i].ready=ready;
            }
        }
        // Do not care about resending atm

        // Resend Package
        if(clients.size()>1){
            Pending pending;
            pending.packet << Packet::Ready << clients[n].id << player[clients[n].id].ready;
            for(unsigned int j=0;j<clients.size();j++){
                if(j!=clients[clients[n].id].id){
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
        std::cout << player[id].name.toAnsiString() << "(" << id << ") " << " lags";
        game.pause=true;
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
}
//
void Server::Sync_Clients(const Config &config,Game &game,const std::vector<Player> &player){
    for(unsigned int i=0;i<player.size();i++){
        Pending pending;
        pending.packet << Packet::Sync << i << player[i].name << player[i].enabled << player[i].ready;
        pending.send_id.push_back(-1);
        game.mutex.lock();
        game.packets.push_back(pending);
        game.mutex.unlock();
    }
}
