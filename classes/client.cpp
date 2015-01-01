// Contains functions and constructors for the Client class
// Needed Header
#include "client.h"
// Functions
void Client::Start(const Config &config, Game &game,std::vector<Player> &player){
    if(player.size()>0){
        player.clear();
    }
    game.client[0]=false;
    std::cout << "Connecting!" << std::endl;
    //for(int i=0; i<config.max_attempts&&!connected;i++){
        sf::Socket::Status status= socket.connect(game.server_ip,config.port,sf::milliseconds(1000));
        if(status==sf::Socket::Error){
            std::cout << "Server connection Error!" << std::endl;
            //sf::sleep(sf::milliseconds(config.attempt_delay));
        }
        else if(status==sf::Socket::NotReady){
             std::cout << "Not ready?!" << std::endl; // This happens for some reason at the first reconnect attempt
             //sf::sleep(sf::milliseconds(config.attempt_delay));
        }
        else{
            std::cout << "Connected to server!" << std::endl;
            socket.setBlocking(false);
            sync=false;
            ready=false;
            //game.connected=true;
            // Start server connection thread
            thread = std::thread(&Client::Thread,this,std::cref(config),std::ref(game),std::ref(player));
            //thread_listener = std::thread(&Server::Server_Listener,this,std::cref(config),std::ref(game),player);
            game.client[1]=true;
            //Initialize_Game_Setup_MP(config,game,player);
            std::cout << "Synchronizing!" << std::endl;
            while(!sync){
                sf::sleep(sf::milliseconds(250));
            }
            std::cout << "Synchronized!" << std::endl;
            game.Switch_Mode(Game::Mode::Setup);
        }
    //}
}
//
void Client::Thread(const Config &config,Game &game,std::vector<Player> &player){
    std::cout << "Client thread started!" << std::endl;
    // Main loop
    while(!game.client[2]){
        // Check if receiving something
        switch(socket.receive(packet)){
            case sf::Socket::Done:
                Process_Packet(config,game,player,packet);
                packet.clear();
                break;
            case sf::Socket::Disconnected:
            case sf::Socket::Error:
                std::cout << "Connection error!" << std::endl;
                // Break connection!
                game.client[2]=true;
                game.mode=Game::Mode::Main_Menu;
                break;
            default:
                break;
        }
        // Send
        for(int i=game.packets.size()-1;i>=0;i--){
            switch(socket.send(game.packets[i].packet)){
                case sf::Socket::Done:
                    // Remove Send id
                    game.mutex.lock();
                    game.packets.erase(game.packets.begin()+i);
                    game.mutex.unlock();
                    break;
                case sf::Socket::NotReady:
                    std::cout << "Server not ready" << std::endl;
                    break;
                case sf::Socket::Disconnected:
                case sf::Socket::Error:
                default:
                    std::cout << "Failed to send package to server"  << std::endl;
                    break;
            } // End switch case
        }
        //sf::sleep(sf::milliseconds(5));
    }
    socket.disconnect();
    game.client[1]=game.client[2]=false;
    game.connected=false;
    std::cout << "Client thread ended!" << std::endl;
}
//
void Client::Ready(Game &game,std::vector<Player> &player){
    game.keychange[0]=-1;
    // Check if keys are set
    if(!ready){
        ready=true;
        for(unsigned int i=0;i<player.size();i++){
            if(player[i].local &&( player[i].keyL==sf::Keyboard::Unknown||player[i].keyR==sf::Keyboard::Unknown) ){
                ready=false;
                player[i].ready=true;
                // Return ? Break?
            }
        }
        // If still ready sent to server
        if(ready){
            Pending pending;
            pending.packet << Packet::Ready << true;
            game.mutex.lock();
            game.packets.push_back(pending);
            game.mutex.unlock();
        }
    }
    else{
        ready=false;
        for(unsigned int i=0;i<player.size();i++){
            if(player[i].local){
                player[i].ready=false;
            }
        }
        //
        Pending pending;
        pending.packet << Packet::Ready << false;
        game.mutex.lock();
        game.packets.push_back(pending);
        game.mutex.unlock();
        player[game.id].ready=false;
    }
}
//
void Client::Process_Packet(const Config &config,Game &game,std::vector<Player> &player,sf::Packet &packet){
    Packet type;
    packet >> type;
    if(type==Packet::ID){
        packet >> game.id;
        while(game.id>=player.size()){
            player.emplace_back("",sf::Color::Black);
            player.back().server=false;
            player.back().ready=false;
        }
        player[game.id].local=true;
    }
    else if(type==Packet::Sync){
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
        sync=true;
        // Switch screen if still IG
        if(game.mode==Game::Mode::Play){
            game.Switch_Mode(Game::Mode::Setup);
        }
    }
    else if(type==Packet::Name){
        int id;
        packet >> id;
        packet >> player[id].name;
        game.refresh_players=true;
    }
    else if(type==Packet::StartGame){
        game.Initialize(config,player);
        std::cout << "Starting Game!!" << std::endl;
    }
    else if(type==Packet::NewRound){
        game.New_Round(config,player);
        // Extract
        int id;
        while(!packet.endOfPacket()){
            packet >> id;
            packet >> player[id].x >> player[id].y >> player[id].heading;
            player[id].New_Round(config,game);
        }
        // Send ready package
        Pending pending;
        pending.packet << Packet::Ready << true;
        game.mutex.lock();
        game.packets.push_back(pending);
        game.mutex.unlock();
    }
    else if(type==Packet::Update){
        // Time Between packets measurement
        game.packettime=game.packetclock.restart().asSeconds();
        // Check for lag
        if(game.packettime>config.lagtime){
            // Send packet
            Pending pending;
            pending.packet << Packet::Lag << game.id;
            game.mutex.lock();
            game.packets.push_back(pending);
            game.mutex.unlock();
        }
        // Unpack Basics
        int number;
        packet >> number;
        if(number>game.packetnumber){
            // Unpack Player Data
            int id;
            while(!packet.endOfPacket()){
                packet >> id;
                player[id].Update_Position(config,packet);
            }
        }
        else if(number<game.packetnumber-5){
            std::cout << "Large packet mixup!! Pausing game!" << ", " << number << ", " << game.packetnumber << std::endl;
            // Send packet
            Pending pending;
            pending.packet << Packet::Lag << game.id;
            game.mutex.lock();
            game.packets.push_back(pending);
            game.mutex.unlock();
        }
        else{
            std::cout << "Packet mixup!!" << ", " << number << ", " << game.packetnumber << std::endl;
        }

    }
    else if(type==Packet::Points){
        int id;
        std::vector<unsigned int> death_vec;
        while(!packet.endOfPacket()){
            packet >> id;
            death_vec.push_back(id);
        }
        game.Player_Death(player,death_vec);
    }
    else if(type==Packet::GameEnd){
        int id;
        packet >> id;
        game.round_winner=id;
        game.game_finished=true;
        game.round_finished=true;
        game.pause=true;
        // Shutdown game thread
        if(game.update_thread[1]){
            game.Shutdown();
        }
    }
    else if(type==Packet::RoundEnd){
        int id;
        packet >> id;
        game.round_winner=id;
        game.round_finished=true;
        game.pause=true;
        game.end_message_set=false;
    }
    else if(type==Packet::PowerupDelF){
        int id;
        packet >> id;
        for(unsigned int i=0;i<game.powerup.size();i++){
            if(game.powerup[i].id==id){
                game.powerup.erase(game.powerup.begin()+i);
                break;
            }
        }
    }
    else if(type==Packet::PowerupDelP){
        int id;
        packet >> id;
        for(unsigned int i=0;i<game.player_powerup_effect.size();i++){
            if(game.player_powerup_effect[i].id==id){
                game.player_powerup_effect.erase(game.player_powerup_effect.begin()+i);
                for(unsigned int j=0;j<player.size();j++){
                    if(!player[j].death){player[j].Calculate_Powerup_Effect(config,game);}
                }
                break;
            }
        }
    }
    else if(type==Packet::PowerupDelG){
        int id;
        packet >> id;
        for(unsigned int i=0;i<game.powerup_effect.size();i++){
            if(game.powerup_effect[i].id==id){
                // Check probably not needed anymore
                if(game.powerup_effect[i].type==7){
                    game.wallsaway=false;
                }
                //
                game.powerup_effect.erase(game.powerup_effect.begin()+i);
                break;
            }
        }
    }
    else if(type==Packet::PowerupS){
        int id;
        packet >> id;
        float D;
        int X,Y,type,impact;
        packet >> X >> Y >> type >> impact >> D;
        Powerup powerup(X,Y,type,impact,D,id);
        game.powerup.push_back(powerup);
    }
    else if(type==Packet::PowerupHit){
        // Player id that hit the powerup
        int id;
        packet >> id;
        // Powerup ID
        int ID;
        packet >> ID;
        // Find and process
        for(unsigned int i=0;i<game.powerup.size();i++){
            if(game.powerup[i].id==ID){
                // Actions
                int D=0;
                if( (game.powerup[i].type!=5&&game.powerup[i].type<7) ||game.powerup[i].type==9 ){
                    game.player_powerup_effect.emplace_back(id,game.powerup[i].type,game.powerup[i].impact,D,ID);
                    for(unsigned int k=0;k<player.size();k++){
                        // Calculate powerup effects
                        player[k].Calculate_Powerup_Effect(config,game);
                    }
                }
                // Clear Line
                else if(game.powerup[i].type==5){
                    for(unsigned int k=0;k<player.size();k++){
                        player[k].line.clear();
                    }
                }
                // Walls Away
                else if(game.powerup[i].type==7){
                    Powerup_Effect effect(game.powerup[i].type,D,game.powerup[i].id);
                    game.powerup_effect.push_back(effect);
                    game.wallsaway=true;
                }
                // Delete
                game.powerup.erase(game.powerup.begin()+i);
                break;
            }
        }
    }
    else if(type==Packet::Pause){
        packet >> game.pause;
        if(!game.pause){
            game.game_clock.restart();
            game.packetclock.restart();
            game.countdown_int=0;
        }
    }
    else if(type==Packet::DCon){
        int id;
        packet >> id;
        // Remove from list
        player.erase(player.begin()+id);
        game.refresh_players=true;
    }
    else if(type==Packet::Ready){
        int id;
        packet >> id;
        packet >> player[id].ready;
        game.refresh_players=true;
    }
    else if(type==Packet::Config){
        packet >> game.maxpoints >> game.powerup_enabled;
    }
    else if(type==Packet::Countdown){
        game.countdown.restart();
        game.countdown_int=3;
        //packet >> game.countdown_int;
    }
}
//
void Client::Shutdown(Game &game){
    game.client[2]=true;
    if(thread.joinable()){
        thread.join();
    }
}
