// Contains functions and constructors for the Server class
// Needed Header
#include "server.h"
// Constructor
Server::Server(): senderPacer(120){}
// Functions
void Server::start(const Config &config,GameSetup &gameSetup,Game &game,std::vector<Player> &player){
    // check if clean
    if(game.server[0]){
        shutdown(game);
    }
    // At least one player is needed for the server at the moment.
    game.id=0;
    if(player.size()==0){
        gameSetup.addPlayer(game,player);
        #ifdef DEBUG
        // Set keys
        player[0].keyL=sf::Keyboard::Left;
        player[0].keyR=sf::Keyboard::Right;
        #endif // DEBUG
    }
    // Create Server Threads
    thread_listener = std::thread(&Server::serverListener,this,std::cref(config),std::ref(gameSetup),std::ref(game),std::ref(player));
    thread_sender = std::thread(&Server::serverSender,this,std::cref(config),std::ref(game),std::ref(player));
    game.serverIp = sf::IpAddress::getLocalAddress().toString();
}
//
void Server::serverListener(const Config &config,GameSetup &gameSetup,Game &game,std::vector<Player> &player){
    std::cout << "Server listener started!!" << std::endl;
    // Init
    if (listener.listen(config.port) == sf::Socket::Done){
        selector.add(listener);
        std::cout << "Bound!" << std::endl;
        game.server[1]=true;
    }
    else{
        game.server[2]=true;
        std::cout << "Can't bind! Please retry." << std::endl;
    }
    // Main Loop
    while(!game.server[2]){
        // Selector
        if(selector.wait(sf::seconds(0.5))){
            // Check Listener
            if(selector.isReady(listener)){
                clients.emplace_back(ClientInfo());
                if (listener.accept(*clients.back().socket) != sf::Socket::Done){
                    std::cout << "Incoming connection failed.\n";
                    clients.pop_back();
                }
                else{
                    // Check if new Clients can be added
                    if(player.size()<config.maxPlayers&&game.mode!=Game::Mode::Play){
                        selector.add(*clients.back().socket);
                        std::cout << "Remote client " + clients.back().socket->getRemoteAddress().toString() + " has connected to the server. \n";
                    }
                    else{
                        // Disconnect
                        std::cout << "Remote client " + clients.back().socket->getRemoteAddress().toString() + " tried to connect. \n";
                        clients.pop_back();
                    }
                }
            }
            // Check clients
            if(clientMutex.try_lock()){
                clientMutex.unlock();
                bool stop=false; // Stop when disconnection!
                for(unsigned int i=0;i<clients.size()&&!stop;i++){
                    if(selector.isReady(*clients[i].socket)){
                        packet.clear();
                        switch(clients[i].socket->receive(packet)){
                            case sf::Socket::Done:
                                // Process Packet
                                processPackage(config,gameSetup,game,player,packet,i);
                                break;
                            case sf::Socket::NotReady:
                                std::cout << "Client not ready?!?" <<std::endl;
                                break;
                            case sf::Socket::Disconnected:
                            case sf::Socket::Error:
                            default:
                                stop=true;
                                std::cout << "Remote client disconnected." << std::endl;
                                clientMutex.lock();
                                disconnectClient(config, gameSetup, game, player, i);
                                clientMutex.unlock();
                                // Find out if all players left
                                if(player.size()==1){
                                    // Return to setup screen
                                    game.quit(config);
                                }
                                break;
                        }
                    }
                } // End Clients
            }// End Selector
        }
    }
    // shutdown
    selector.clear();
    clients.clear();
    listener.close();
    game.server[1]=false;
    game.server[0]=true;
    // Remove non local clients
    for(unsigned int i=0;i<player.size();i++){
        if(!player[i].local){
            gameSetup.removePlayer(game,player,i);
            i--;
        }
    }
    //
    std::cout << "Server listener ended!" << std::endl;
}
//
void Server::serverSender(const Config &config,Game &game,std::vector<Player> &player){
    std::cout << "Server Sender started!!" << std::endl;
    // Main Loop
    while(!game.server[2]){
        // check if there are packages
        for(unsigned int i=0;i<game.packets.size();i++){
            // Remove packet if no clients
            if(clients.size()==0){
                game.packetMutex.lock();
                game.packets.erase(game.packets.begin()+i);
                game.packetMutex.unlock();
                i--;
                continue;
            }
            // Send to all if sendID[0]=-1;
            else if(game.packets[i].sendID[0]==-1){
                game.packets[i].sendID.clear();
                for(unsigned int j=0;j<clients.size();j++){
                    game.packets[i].sendID.push_back(j);
                }
            }
            // Send loop
            for(int j=game.packets[i].sendID.size()-1;j>=0;j--){
                // check if client still exists
                if(game.packets[i].sendID[j]>=clients.size()){
                    game.packets[i].sendID.erase(game.packets[i].sendID.begin()+j);
                }
                else{
                    if(clientMutex.try_lock()){
                        switch(clients[game.packets[i].sendID[j]].socket->send(game.packets[i].packet)){
                            case sf::Socket::Done:
                                // Remove Send id
                                game.packets[i].sendID.erase(game.packets[i].sendID.begin()+j);
                                break;
                            case sf::Socket::NotReady:
                                std::cout << "Client not ready: " << clients[game.packets[i].sendID[j]].socket->getRemoteAddress().toString()  << std::endl;
                                break;
                            case sf::Socket::Disconnected:
                            case sf::Socket::Error:
                            default:
                                std::cout << "Failed to send packet:" << clients[game.packets[i].sendID[j]].socket->getRemoteAddress().toString()  << std::endl;
                                break;
                        } // End toggle case
                        clientMutex.unlock();
                    }
                }
            } // End Send id for loop
            // Remove empty packets
            if(game.packets[i].sendID.size()==0){
                // Check type; If update package update game.packetTime.
                Packet type;
                game.packets[i].packet >> type;
                if(type==Packet::Update&&++game.packetNumber2%config.gameUpdateThreadMinRate==0){
                    game.packetTime=game.packetClock.restart().asSeconds();
                }
                //
                game.packetMutex.lock();
                game.packets.erase(game.packets.begin()+i);
                game.packetMutex.unlock();
                i--;
            }
        } // End Loop
        senderPacer.pace();
    }
    // shutdown
    std::cout << "Server Sender ended!" << std::endl;
}
// Great to use a variable named new client that can be false and the function name is New_Client
void Server::newClient(const Config &config,GameSetup &gameSetup,Game &game,std::vector<Player> &player, const unsigned int &n, const bool &new_client){
    //
    clients[n].versionCorrect=true;
    // Add new player and synchronize positions between client_info and player vector
    gameSetup.addPlayer(game,player);
    clients[n].id.push_back(player.size()-1);
    player.back().id=n;
    player.back().local=false;
    player.back().server=false;
    // Send everything to clients
    syncClients(game,player);
    sf::sleep(sf::milliseconds(250)); // Fixes some errors
    if(new_client){
        Pending pending;
        unsigned int temp=player.size()-1; // Needed for some reason. Problem with int's?; Should change to fixed size int's
        pending.packet << Packet::ID << temp;
        pending.sendID.push_back(n);
        game.queuePacket(pending);
    }
    else{
        Pending pending;
        unsigned int temp=player.size()-1;
        pending.packet << Packet::RequestPlayer << temp;
        pending.sendID.push_back(n);
        game.queuePacket(pending);
    }
    game.optionsChanged();
}
//
void Server::disconnectClient(const Config &config, GameSetup &gameSetup, Game &game,std::vector<Player> &player, const unsigned int &n){
    // Create Package and remove player
    Pending pending;
    pending.packet << Packet::Disconnect;
    for(unsigned int j=0;j<clients[n].id.size();j++){
        pending.packet << clients[n].id[j];
        gameSetup.removePlayer(game,player,clients[n].id[j]);
    }
    // Remove out of list
    selector.remove(*clients[n].socket);
    clients.erase(clients.begin()+n);
    // Update id in player and clientInfo
    for(unsigned int j=1;j<player.size();j++){
        if(!player[j].local&&player[j].id>n){
            player[j].id--;
            clients[player[j].id].updateID(j+1);
        }
    }
    // Pend Packed
    game.queuePacket(pending);
    // Check game end
    if(game.deathCount>=player.size()-1){
        // End Round function
        game.endRound(config,player);
    }
}
// n is the place in the client vector
void Server::processPackage(const Config &config,GameSetup &gameSetup,Game &game,std::vector<Player> &player,sf::Packet &packet, const unsigned int &n){
    Packet type;
    packet >> type;
    //
    if(!clients[n].versionCorrect){
        if(type==Packet::Version){
            std::string version;
            packet >> version;
            if(version==config.majorVersion){
                std::cout << "Client version correct!" << std::endl;
                newClient(config,gameSetup,game,player,n,true);
            }
            else{
                std::cout << "Incorrect client version!" << std::endl;
                Pending pending;
                pending.packet << Packet::Version << false;
                pending.sendID.push_back(n);
                game.queuePacket(pending);
                sf::sleep(sf::seconds(0.5));
                disconnectClient(config, gameSetup,game,player,n);
            }
        }
        else{
            // Force disconnect
            disconnectClient(config, gameSetup,game,player,n);
        }
    }
    //
    else if(type==Packet::Name){
        int id;
        packet >> id;
        // Data check
        if(!clients[n].checkID(id)){
            return;
        }
        // Update
        packet >> player[id].name;
        // Resend Package
        if(clients.size()>1){
            Pending pending;
            pending.packet << Packet::Name << id << player[id].name;
            for(unsigned int j=0;j<clients.size();j++){
                if(j!=clients[j].id[0]){
                    pending.sendID.push_back(j);
                }
            }
            game.queuePacket(pending);
        }
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
                    pending.sendID.push_back(j);
                }
            }
            game.queuePacket(pending);
        }
    }
    else if(type==Packet::Key){
        int id;
        packet >> id;
        // Data check
        if(!clients[n].checkID(id)){
            return;
        }
        //
        packet >> player[id].left >> player[id].right;
    }
    else if(type==Packet::Lag){
        int packetNumber, id;
        packet >> id >> packetNumber;
        // Check if packetNumber is really far behing
        int lagNumber  = game.packetNumber - config.gameUpdateThreadMinRate * (config.lagTime*2/3); // 2/3 to compensate for travel time and such
        if(packetNumber <= lagNumber){
            // Really lagging
            std::cout << player[id].name.toAnsiString() << "(" << id << ") " << " lags" << std::endl;
            game.pause(true);
        }
        else{
            std::cout << player[id].name.toAnsiString() << "(" << id << ") " << " does not lag" << std::endl;
            // Respond
            Pending pending;
            pending.packet << Packet::Lag << false;
            pending.sendID.push_back(n);
            game.queuePacket(pending);
        }
    }
    else if(type==Packet::RequestPlayer){
        // check if there are free players
        if(player.size()==config.maxPlayers||!game.multiplePlayersEnabled){
            // If not then mention it to the client
            Pending pending;
            pending.packet << Packet::RequestPlayer << -1;
            pending.sendID.push_back(n);
            game.queuePacket(pending);
            return;
        }
        // Create new player
        newClient(config,gameSetup,game,player,n,false);
    }
    else if(type==Packet::RemovePlayer){
        unsigned int id;
        packet >> id;
        // Data check
        if(!clients[n].checkID(id)){
            return;
        }
        // Remove
        gameSetup.removePlayer(game,player,id);
        //
        // Remove from clients
        for(unsigned int j=0;j<clients[n].id.size();j++){
            if(clients[n].id[j]==id){
                clients[n].id.erase(clients[n].id.begin()+j);
                break;
            }
        }
        // Update player id's
        updatePlayerID(player,id);
        // Send to other clients
        Pending pending;
        pending.packet << Packet::Disconnect << id;
        game.queuePacket(pending);
    }
}
//
void Server::shutdown(Game &game){
    game.server[2]=true;
    if(thread_listener.joinable()){
        thread_listener.join();
    }
    if(thread_sender.joinable()){
        thread_sender.join();
    }
    game.server[0]=game.server[2]=false;
}
//
void Server::syncClients(Game &game,const std::vector<Player> &player){
    // All player information
    Pending pending;
    pending.packet << Packet::Sync;
    for(unsigned int i=0;i<player.size();i++){
        if(clients.size()<player[i].id){
            pending.packet << i << player[i].name << player[i].color << player[i].server << false;
        }
        else{
            pending.packet << i << player[i].name << player[i].color << player[i].server << clients[player[i].id].ready;
        }

    }
    game.queuePacket(pending);
}
// n denotes the player that will be removed
void Server::updatePlayerID(std::vector<Player> &player,const unsigned int &n){
    // First update id in clients
    for(unsigned int i=0;i<clients.size();i++){
        for(unsigned int j=0;j<clients[i].id.size();j++){
            if(clients[i].id[j]>n){
                clients[i].id[j]--;
            }
            player[clients[i].id[j]].id=i;
        }
    }
}
//
