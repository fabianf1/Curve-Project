// Contains the client handler
// Headers
#include "../curve.h"
//
void Start_Client(const Config &config, Game &game,Player player[]){
    game.client[0]=false;
    for(int i=0; i<config.max_attempts&&!game.connected;i++){
        sf::Socket::Status status= game.socket.connect(sf::IpAddress(game.server_ip),config.port,sf::milliseconds(500));
        if(status==sf::Socket::Error){
            std::cout << "Server connection Error!" << std::endl;
            sf::sleep(sf::milliseconds(config.attempt_delay));
        }
        else if(status==sf::Socket::NotReady){
             std::cout << "Not ready?!" << std::endl; // This happens for some reason at the first reconnect attempt
             sf::sleep(sf::milliseconds(config.attempt_delay));
        }
        else{
            std::cout << "Connected to server!" << std::endl;
            game.socket.setBlocking(false);
            game.connected=true;
            // Start server connection thread
            game.threads.emplace_back(Client_Thread,std::cref(config),std::ref(game),player);
            game.client[1]=true;
            Initialize_Game_Setup_MP(config,game,player);
        }
    }
}
//
void Client_Thread(const Config &config,Game &game,Player player[]){
    std::cout << "Client thread started!" << std::endl;
    sf::Packet packet;
    // Main loop
    while(!game.client[2]){
        // Check if receiving something
        switch(game.socket.receive(packet)){
            case sf::Socket::Done:
                Client_Process_Packet(config,game,player,packet);
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
        for(int i=game.pending.size()-1;i>=0;i--){
            switch(game.socket.send(game.pending[i].packet)){
                case sf::Socket::Done:
                    //std::cout << "Package Send to server" << std::endl;
                    //std::cout << server.clients[0].ip.toString() << std::endl;
                    // Remove Send id
                    game.mutex.lock();
                    game.pending.erase(game.pending.begin()+i);
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
    game.socket.disconnect();
    game.client[1]=game.client[2]=false;
    game.connected=false;
    game.socket.setBlocking(true);
    std::cout << "Client thread ended!" << std::endl;
}
//
void Client_Ready(Game &game,Player player[]){
    game.keychange[1]=-1;
    // Check if keys are set
    if(!player[game.id].ready&&player[game.id].keyL!=sf::Keyboard::Unknown&&player[game.id].keyR!=sf::Keyboard::Unknown){
        // Package
        Pending pending;
        pending.packet << Packet::Ready << game.id << true;
        game.mutex.lock();
        game.pending.push_back(pending);
        game.mutex.unlock();
        // Local
        player[game.id].ready=true;
    }
    else if(player[game.id].ready){
        Pending pending;
        pending.packet << Packet::Ready << game.id << false;
        game.mutex.lock();
        game.pending.push_back(pending);
        game.mutex.unlock();
        player[game.id].ready=false;
    }
}
//
void Client_Process_Packet(const Config &config,Game &game,Player player[],sf::Packet &packet){
    //std::cout << "Hello";
    Packet type;
    //std::cout << "Hello2";
    packet >> type;
    //std::cout << "Hello3";
    if(type==Packet::ID){
        packet >> game.id;
    }
    else if(type==Packet::Sync){
        int id;
        packet >> id;
        player[id].Process_SYNC_Packet(packet);
    }
    else if(type==Packet::Name){
        int id;
        packet >> id;
        packet >> player[id].name;
    }
    else if(type==Packet::StartGame){
        Initialize_Game_Client(config,game,player);
        std::cout << "Starting Game!!" << std::endl;
    }
    else if(type==Packet::NewRound){
        if(game.round_finished){
            Initialize_New_Round_Client(config,game,player);
        }
        int id;
        packet >> id;
        player[id].New_Round_Client(config,game,packet);
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
            game.pending.push_back(pending);
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
                player[id].Update_Position_Client(config,packet);
            }
        }
        else{
            std::cout << "Packet Mixup!!" << ", " << number << ", " << game.packetnumber << std::endl;
        }

    }
    else if(type==Packet::Points){
        int id;
        std::vector<int> death_vec;
        while(!packet.endOfPacket()){
            packet >> id;
            death_vec.push_back(id);
        }
        Add_Points(player,death_vec);
        for(unsigned int i=0;i<death_vec.size();i++){
            player[i].death=true;
        }
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
            game.update_thread[2]=true;
        }
    }
    else if(type==Packet::RoundEnd){
        int id;
        packet >> id;
        game.round_winner=id;
        game.round_finished=true;
        game.pause=true;
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
                for(int j=0;j<MAX_PLAYERS;j++){
                    if(player[j].enabled&&!player[j].death){player[j].Calculate_Powerup_Effect(config,game);}
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
                    for(int k=0;k<MAX_PLAYERS;k++){
                        // Calculate powerup effects
                        player[k].Calculate_Powerup_Effect(config,game);
                    }
                }
                // Clear Line
                else if(game.powerup[i].type==5){
                    for(unsigned int k=0;k<MAX_PLAYERS;k++){
                        if(!player[k].enabled){continue;}
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
        player[id].enabled=false;
    }
    else if(type==Packet::Ready){
        int id;
        packet >> id;
        packet >> player[id].ready;
    }
    else if(type==Packet::Config){
        packet >> game.maxpoints >> game.powerup_enabled;
    }
    else if(type==Packet::Countdown){
        packet >> game.countdown_int;
    }
}
