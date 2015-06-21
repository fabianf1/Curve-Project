// Contains functions and constructors for the Client class
// Needed Header
#include "Client.h"
// Constructor
Client::Client(): pacer(200){}
// Functions
void Client::start(const Config &config, Game &game,std::vector<Player> &player){
    if(player.size()>0){
        player.clear();
    }
    // check if clean
    if(game.client[0]){
        shutdown(game);
    }
    socket.setBlocking(true);
    sf::Socket::Status status= socket.connect(game.serverIp,config.port,sf::milliseconds(config.attemptDuration));
    if(status==sf::Socket::Error){
        std::cout << "Server connection Error!" << std::endl;
    }
    else if(status==sf::Socket::NotReady){
        // When the client tries to reconnect this always occurs for some reason. Trying again results in a succeeded connection attempt.
        // This was fixed by making the socket block again
        std::cout << "No response from server." << std::endl;
    }
    else{
        std::cout << "Connected to server! Checking version..." << std::endl;
        socket.setBlocking(false);
        sync=false;
        ready=false;
        // start server connection thread
        clientThread = std::thread(&Client::thread,this,std::cref(config),std::ref(game),std::ref(player));
        game.client[1]=true;
    }
}
//
void Client::thread(const Config &config,Game &game,std::vector<Player> &player){
    std::cout << "Client thread started!" << std::endl;
    // Send version check
    Pending pending;
    pending.packet << Packet::Version << config.majorVersion;
    game.queuePacket(pending);
    // Main loop
    while(!game.client[2]){
        // check if receiving something
        switch(socket.receive(packet)){
            case sf::Socket::Done:
                processPacket(config,game,player,packet);
                packet.clear();
                break;
            case sf::Socket::Disconnected:
            case sf::Socket::Error:
                if(!sync){
                    std::cout << "Server full!" << std::endl;
                }
                else{
                    std::cout << "Disconnected from server!" << std::endl;
                }
                // Break connection!
                game.client[2]=true;
                game.mode=Game::Mode::mainMenu;
                break;
            default:
                break;
        }
        // Send
        for(int i=game.packets.size()-1;i>=0;i--){
            switch(socket.send(game.packets[i].packet)){
                case sf::Socket::Done:
                    // Remove Send id
                    game.packetMutex.lock();
                    game.packets.erase(game.packets.begin()+i);
                    game.packetMutex.unlock();
                    break;
                case sf::Socket::NotReady:
                    std::cout << "Server not ready" << std::endl;
                    break;
                case sf::Socket::Disconnected:
                case sf::Socket::Error:
                default:
                    std::cout << "Failed to send package to server"  << std::endl;
                    break;
            } // End toggle case
        }
        //
        pacer.pace();
    }
    socket.disconnect();
    game.client[1]=game.client[2]=false;
    game.client[0]=true;
    player.clear();
    std::cout << "Client thread ended!" << std::endl;
}
//
void Client::toggleReady(Game &game,std::vector<Player> &player){
    game.keyChange[0]=-1;
    // check if keys are set
    if(!ready){
        ready=player[game.id].ready=true;
        for(unsigned int i=0;i<player.size();i++){
            if(player[i].local &&( player[i].keyL==sf::Keyboard::Unknown||player[i].keyR==sf::Keyboard::Unknown) ){
                ready=player[game.id].ready=false;
                break;
            }
        }
        // If still ready sent to server
        if(ready){
            Pending pending;
            pending.packet << Packet::Ready << true;
            game.queuePacket(pending);
        }
    }
    else{
        ready=player[game.id].ready=false;
        // Send packet
        Pending pending;
        pending.packet << Packet::Ready << false;
        game.queuePacket(pending);
    }
}
//
void Client::processPacket(const Config &config,Game &game,std::vector<Player> &player,sf::Packet &packet){
    Packet type;
    packet >> type;
    // If waiting for sync only accept that and version package
    if(!sync){
        // Version is correct!
        if(type==Packet::Sync){
            syncPackage(game,player,packet);
            std::cout << "Version correct!" << std::endl;
            //
            #ifdef DEBUG
            // Set keys
            player[1].keyL=sf::Keyboard::Z;
            player[1].keyR=sf::Keyboard::X;
            #endif // DEBUG
            game.switchMode(Game::Mode::setup);
        }
        // Version is incorrect
        else if(type==Packet::Version){
            bool correct;
            packet >> correct;
            if(!correct){
                // Server has different game version. Disconnect!
                std::cout << "Server has different version. Disconnecting!" << std::endl;
                game.client[2]=true;
            }
        }
        else{
            std::cout << "Wrong package..." << std::endl;
        }
    }
    else if(type==Packet::ID){
        packet >> game.id;
        while(game.id>=player.size()){
            player.emplace_back("",sf::Color::Black);
            player.back().server=false;
            player.back().ready=false;
        }
        player[game.id].local=true;
    }
    else if(type==Packet::Sync){
        syncPackage(game,player,packet);
    }
    else if(type==Packet::Name){
        int id;
        packet >> id;
        packet >> player[id].name;
        game.refreshPlayers=true;
    }
    else if(type==Packet::StartGame){
        game.initialize(config,player);
    }
    else if(type==Packet::NewRound){
        game.newRound(config,player);
        // Extract
        int id;
        while(!packet.endOfPacket()){
            packet >> id;
            packet >> player[id].x >> player[id].y >> player[id].heading;
            player[id].newRound(config,game);
        }
    }
    else if(type==Packet::Update){
        // Time Between packets measurement
        if(game.packetNumber%config.gameUpdateThreadMinRate==0){
            game.packetTime=game.packetclock.restart().asSeconds();
            // check for lag
            if(game.packetTime>config.lagTime){
                // Send packet
                Pending pending;
                pending.packet << Packet::Lag << game.id;
                game.queuePacket(pending);
            }
        }
        // Unpack Basics
        int number;
        packet >> number;
        if(number>game.packetNumber){
            game.packetNumber=number;
            // Unpack Player Data
            int id;
            while(!packet.endOfPacket()){
                packet >> id;
                player[id].updatePosition(config,packet);
            }
        }
        // This should not happen anymore
        else{
            std::cout << "Packet mixup!!" << ", " << number << ", " << game.packetNumber << std::endl;
        }

    }
    else if(type==Packet::Points){
        int id;
        std::vector<unsigned int> death_vec;
        while(!packet.endOfPacket()){
            packet >> id;
            death_vec.push_back(id);
        }
        game.playerDeath(player,death_vec);
    }
    else if(type==Packet::GameEnd){
        int id;
        packet >> id;
        game.roundWinner=id;
        game.gameFinished=true;
        game.roundFinished=true;
        game.pause(true);
        // shutdown game thread
        if(game.updateThread[1]){
            game.shutdown();
        }
    }
    else if(type==Packet::RoundEnd){
        int id;
        packet >> id;
        game.roundWinner=id;
        game.roundFinished=true;
        game.pause(true);
        game.endMessageSet=false;
    }
    else if(type==Packet::PowerupDeleteField){
        int id;
        packet >> id;
        for(unsigned int i=0;i<game.powerupField.size();i++){
            if(game.powerupField[i].id==id){
                game.powerupField.erase(game.powerupField.begin()+i);
                break;
            }
        }
    }
    else if(type==Packet::PowerupDeletePlayer){
        int id;
        packet >> id;
        for(unsigned int i=0;i<game.playerPowerupEffect.size();i++){
            if(game.playerPowerupEffect[i].id==id){
                game.playerPowerupEffect.erase(game.playerPowerupEffect.begin()+i);
                for(unsigned int j=0;j<player.size();j++){
                    if(!player[j].death){player[j].calculatePowerupEffect(config,game);}
                }
                break;
            }
        }
    }
    else if(type==Packet::PowerupSpawn){
        int id;
        packet >> id;
        float D;
        int X,Y;
        Powerup::Type type;
        Powerup::Impact impact;
        packet >> X >> Y >> type >> impact >> D;
        game.powerupField.emplace_back(X,Y,type,impact,D,id);
    }
    else if(type==Packet::PowerupHit){
        // check
        if(game.roundFinished){
            return;
        }
        // Player id that hit the powerup
        int id;
        packet >> id;
        // Powerup ID
        int ID;
        packet >> ID;
        // Find and process
        for(unsigned int i=0;i<game.powerupField.size();i++){
            if(game.powerupField[i].id==ID){
                // Actions
                // Most things are calculated server side and only the powerup has to be removed from the field.
                int D=0;
                switch (game.powerupField[i].type){
                    case Powerup::Type::RightAngle:
                    case Powerup::Type::InvertKeys:
                        game.playerPowerupEffect.emplace_back(id,game.powerupField[i].type,game.powerupField[i].impact,D,ID);
                        for(unsigned int k=0;k<player.size();k++){
                            // Calculate powerup effects
                            player[k].calculatePowerupEffect(config,game);
                        }
                        break;
                    case Powerup::Type::Clear:
                        for(unsigned int k=0;k<player.size();k++){
                            player[k].line.clear();
                        }
                        break;
                    case Powerup::Type::WallsAway:
                        game.wallsAway=true;
                        break;
                    case Powerup::Type::Darkness:
                        game.darkness=true;
                        break;
                    case Powerup::Type::Bomb:
                        game.powerUpBomb(config,player,id,i);
                        break;
                    default:
                        break;
                }
                // Delete
                game.powerupField.erase(game.powerupField.begin()+i);
                break;
            }
        }
    }
    else if(type==Packet::PowerupEnd){
        Powerup::Type type;
        packet >> type;
        if(type==Powerup::Type::Darkness){
            game.darkness=false;
        }
        else if(type==Powerup::Type::WallsAway){
            game.wallsAway=false;
        }
    }
    else if(type==Packet::Pause){
        bool pause;
        packet >> pause;
        game.pause(pause);
    }
    else if(type==Packet::Disconnect){
        int id;
        int counter=0;
        while(!packet.endOfPacket()){
            packet >> id;
            // check if not self
            if( (game.mode==Game::Mode::Play&&!player[id].local)||(game.mode!=Game::Mode::Play) ) {
                // Remove from list
                player.erase(player.begin()+id);
                if(game.id>id){
                    game.id--;
                }
            }
            counter++;
        }
        if(counter==1){
            game.removedPlayer=id;
        }
        game.refreshPlayers=true;
    }
    else if(type==Packet::Ready){
        int id;
        while(!packet.endOfPacket()){
            packet >> id;
            packet >> player[id].ready;
        }
        game.refreshPlayers=true;
    }
    else if(type==Packet::Options){
        packet >> game.maxPoints >> game.powerupEnabled >> game.multiplePlayersEnabled;
        game.refreshOptions=true;
    }
    else if(type==Packet::Countdown){
        game.countdownClock.restart();
        game.countdownInt=3;
    }
    else if(type==Packet::ReturnSetup){
        if(game.updateThread[1]){
            game.shutdown();
        }
        ready=false;
        Pending pending;
        pending.packet << Packet::Ready << false;
        game.queuePacket(pending);
        game.switchMode(Game::Mode::setup);
    }
    else if(type==Packet::RequestPlayer){
        int id;
        packet >> id;
        //
        if(id==-1){
            std::cout << "Request denied!" << std::endl;
        }
        else{
            while(id>=player.size()){
                player.emplace_back("",sf::Color::Black);
                player.back().server=false;
                player.back().ready=false;
            }
            player[id].local=true;
            game.refreshPlayers=true;
        }
    }
    else{
        std::cout << "Unknown packet type!" << std::endl;
    }
}
//
void Client::syncPackage(Game &game,std::vector<Player> &player,sf::Packet &packet){
    unsigned int id;
    sf::String name;
    sf::Color color;
    bool server,ready;
    while(!packet.endOfPacket()){
        packet >> id >> name >> color >> server >> ready;
        // Update
        if(id>=player.size()){
            player.emplace_back(name,color);
            player.back().server=server;
            player.back().ready=ready;
        }
        else{
            player[id].name=name;
            player[id].color=color;
            player[id].server=server;
            player[id].ready=ready;
        }
    }
    game.refreshPlayers=true;
    sync=true;
}
//
void Client::shutdown(Game &game){
    game.client[2]=true;
    if(clientThread.joinable()){
        clientThread.join();
    }
    game.client[0]=game.client[2]=false;
}
