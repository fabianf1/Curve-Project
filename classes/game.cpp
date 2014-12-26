// Contains functions and constructors for the config class
// Needed Header
#include "game.h"
// Constructor
Game::Game(){
    frame=0;
    lastclick=0;
    keychange[0]=-1;
    name_change=false;
    maxpoints=5;
    mode=Mode::Main_Menu;
    powerup_enabled=true;
    player_switched=false;
    morepowerups=0;
    update_thread[0]=update_thread[1]=update_thread[2]=false;
    server[0]=server[1]=server[2]=false;
    client[0]=client[1]=client[2]=false;
    online=false;
    connected=false;
    server_ip=sf::IpAddress::getLocalAddress().toString();
    countdown_int=0;
    srand (std::time(nullptr));
}
// Functions
void Game::Switch_Mode(const Game::Mode &Mode){
    // Lock Mutex
    //mode_lock.lock();
    // Switch mode
    mode=Mode;
    //
    //mode_lock.unlock();
    sf::sleep(sf::seconds(0.25));
}
//
void Game::Initialize(const Config &config,Game &game, std::vector<Player> &player){
    // Send start game packet to clients
    if(game.server[1]){
        Pending pending;
        pending.packet << Packet::StartGame;
        pending.send_id.push_back(-1);
        game.mutex.lock();
        game.packets.push_back(pending);
        game.mutex.unlock();
    }
    //
    player_powerup_effect.clear();
    // Make players ready for new game
    for(unsigned int i=0;i<player.size();i++){
        if(!game.client[1]){
            player[i].New_Game(config,game);
        }
        player[i].ready=false;
        player[i].points=0;
    }
    round=0;
    packetnumber=0;
    // Start update thread
    thread = std::thread(&Game::Thread,this,std::cref(config),std::ref(game),std::ref(player));
    // At last switch to game screen
    Switch_Mode(Game::Mode::Play);
    //
    if(!game.client[1]){
        New_Round(config,game,player);
    }
}
//
void Game::Thread(const Config &config,Game &game,std::vector<Player> &player){
    std::cout << "Game update thread Started" << std::endl;
    update_thread[1]=true;
    game_update_thread_clock.restart();
    while(!update_thread[2]){
        // Only run if not paused and not round finished
        if(game.countdown_int!=0){
            float elapsed = 3.0 - game.countdown.getElapsedTime().asSeconds();
            if(elapsed<=0.0){
                game.countdown_int=0;
                Pause(config,false);
            }
            else if(elapsed <=1.0){
                game.countdown_int=1;
            }
            else if(elapsed <=2.0){
                game.countdown_int=2;
            }
        }
        else if( !game.client[1] && !pause && !round_finished ){
            // Elapsed
            elapsed=game_clock.restart().asSeconds();
            if(elapsed>config.max_dt){
                elapsed=config.max_dt;
            }
            // End Elapsed
            // Powerup
            if(game.powerup_enabled){PowerUp_Manager(config,game,player);}
            // Update
            Pending pending;
            if(game.server[1]){
                pending.send_id.push_back(-1);
                game.packetnumber++;
                pending.packet << Packet::Update << game.packetnumber;
            }
            for(unsigned int i=0;i<player.size();i++){
                if(!player[i].death){
                    player[i].Update_Position(config,game);
                    if(game.server[1]){
                        pending.packet << i << player[i].x << player[i].y << player[i].heading << (player[i].invisible||player[i].gap[0]<0.0) ;
                    }
                }
            }
            if(game.server[1]){
                game.mutex.lock();
                game.packets.push_back(pending);
                game.mutex.unlock();
                game.send=false;
                //
                game.packettime=game.packetclock.restart().asSeconds();
            }
            // End Update
            // Hit detection
            Hit_Detector(config,player);
        }
        else if(game.client[1]&&!round_finished){
            // Check key states and send if changed
            // KeyL changed
            if( (!player[game.id].left&&sf::Keyboard::isKeyPressed(player[game.id].keyL)) || (player[game.id].left&&!sf::Keyboard::isKeyPressed(player[game.id].keyL)) ){
                player[game.id].left=sf::Keyboard::isKeyPressed(player[game.id].keyL);
                // Send package
                Pending pending;
                pending.packet << Packet::KeyL << game.id << player[game.id].left;
                game.mutex.lock();
                game.packets.push_back(pending);
                game.mutex.unlock();
            }
            // KeyR changed
            if( (!player[game.id].right&&sf::Keyboard::isKeyPressed(player[game.id].keyR)) || (player[game.id].right&&!sf::Keyboard::isKeyPressed(player[game.id].keyR)) ){
                player[game.id].right=sf::Keyboard::isKeyPressed(player[game.id].keyR);
                // Send package
                Pending pending;
                pending.packet << Packet::KeyR << game.id << player[game.id].right;
                game.mutex.lock();
                game.packets.push_back(pending);
                game.mutex.unlock();
            }
            // Powerup
            if(!game.pause){
                game.elapsed=game.game_clock.restart().asSeconds();
                PowerUp_Manager(config,game);
            };
        }
        Sleep(config.game_update_thread_min_time,game_update_thread_clock);
        game.frame++;
    }
    // Shutdown
    update_thread[0]=false;
    update_thread[1]=false;
    update_thread[2]=false;
    std::cout << "Game update thread Ended" << std::endl;
}
//
void Game::New_Round(const Config &config,Game &game,std::vector<Player> &player){
    player_powerup_effect.clear();
    // Random position for the players and heading
    for(unsigned int i=0;i<player.size();i++){
        if(!game.client[1]){
            player[i].New_Round(config,game);
        }
        player[i].place=i;
        player[i].left=player[i].right=false;
    }
    // Send data
    if(game.server[1]){
        // Send data to clients
        Pending pending;
        pending.packet << Packet::NewRound;
        for(unsigned int i=0;i<player.size();i++){
            pending.packet << i << player[i].x << player[i].y << player[i].heading;
        }
        pending.send_id.push_back(-1);
        game.mutex.lock();
        game.packets.push_back(pending);
        game.mutex.unlock();
    }
    // Reset Some vars
    deathcount=0;
    round_finished=false;
    game_finished=false;
    end_message_set=false;
    pause=true; // Pause_Game(config,game,window,true);
    round++;
    powerup.clear();
    powerup_effect.clear();
    wallsaway=false;
    morepowerups=0;
    powerup_spawn_time=config.powerup_spawn_delay;
    // Ready 'm up!
    if(game.server[1]){
        // I will need a better method if I want this
        /*bool ready=false;
        while(!ready){
            ready=true;
            for(unsigned int i=0;i<player.size();i++){
                if(!player[i].ready){
                    ready=false;
                    break;
                }
            }
        }*/
        // Start countdown
        game.countdown.restart();
        game.countdown_int=3;
        Pending pending;
        pending.send_id.push_back(-1);
        pending.packet << Packet::Countdown;
        game.mutex.lock();
        game.packets.push_back(pending);
        game.mutex.unlock();
    }
    // Could implement client side countdown here
}
//
void Game::Hit_Detector(const Config &config,std::vector<Player> &player){
    // Initializer vector to store death people
    std::vector<unsigned int> death_vec;
    // Do the magic
    for(unsigned int i=0;i<player.size();i++){
        if(player[i].death){continue;}
        // Player Position
        float x=player[i].x;
        float y=player[i].y;
        // Out of bounds check, Only if there are walls
        if(!wallsaway&& (x-player[i].linewidth/2<config.wallwidth||x+player[i].linewidth/2>config.window_width-config.statuswidth-config.wallwidth||y-player[i].linewidth/2<config.wallwidth||y+player[i].linewidth/2>config.window_height-config.wallwidth) ){
            death_vec.emplace_back(i);
        }
        else{
            // Line hit check
            // Only when not gapping
            if(player[i].gap[0]>0.0&&!player[i].invisible){
                float diffx,diffy;
                //
                for(unsigned int j=0;j<player.size();j++){
                    bool hit=false;
                    // Other Player
                    if(j!=i){
                        // Normal Detection
                        for(unsigned int k=0;k<player[j].line.getVertexCount();k++){
                            diffx=x-player[j].line[k].position.x;
                            diffy=y-player[j].line[k].position.y;
                            if( (diffx*diffx) + (diffy*diffy) < player[i].linewidth*player[i].linewidth/4 ){
                                death_vec.emplace_back(i);
                                hit=true;
                                break;
                            }
                        }
                        // Head on detection
                        // Only Fire when not gapping
                        if(!hit&&player[j].gap[0]>0.0&&!player[j].invisible){
                            diffx=x-player[j].x;
                            diffy=y-player[j].y;
                            float radius=player[i].linewidth/2.0+player[j].linewidth/2.0;
                            if( (diffx*diffx) + (diffy*diffy) < (radius*radius) ){
                                death_vec.emplace_back(i);
                                break;
                            }
                        }

                    }
                    // Own line
                    else if(j==i&&player[j].line.getVertexCount()>30){
                        // Safety feature if big and small
                        int delay=30;
                        if(player[j].linewidth>config.linewidth&&player[j].shift<config.shift&&player[j].line.getVertexCount()>45){
                            delay=45;
                        }
                        for(unsigned int k=0;k<player[j].line.getVertexCount()-delay;k++){
                            diffx=x-player[j].line[k].position.x;
                            diffy=y-player[j].line[k].position.y;
                            if( (diffx*diffx) + (diffy*diffy) < player[i].linewidth*player[i].linewidth/4 ){
                                death_vec.emplace_back(i);
                                break;
                            }
                        }
                    } // End own line hit check
                } // End for loop
            } // End frame>gap
        }// End else
    } // End for loop
    // Process the death people
    if(death_vec.size()>0){
        //
        Player_Death(player,death_vec);
        // Stop round if less then two players
        if(deathcount>player.size()){ // For debugging purposes
            std::cout << "Too many deaths!" << std::endl;
            End_Round(config,player);
        }
        else if(deathcount>=player.size()-1){
            // End Round function
            End_Round(config,player);
        }
    }
}
//
void Game::Player_Death(std::vector<Player> &player,std::vector<unsigned int> &death_vec){
    /*if(game.server[1]){
            Add_Points_Server(game,player,death_vec);
            // Make sure last position is send
            if(game.send){
                for(unsigned int i=0;i<death_vec.size();i++){
                    player[death_vec[i]].sendlast=true;
                }
            }
    }*/
    Add_Points(player,death_vec);
    // Do these actions for all dead people
    for(unsigned int i=0;i<death_vec.size();i++){
        deathcount++;
        player[death_vec[i]].death=true;
        player[death_vec[i]].deathframe=frame;
    }
}
//
void Game::Add_Points(std::vector<Player> &player, std::vector<unsigned int> &death_vec){
    for(unsigned int j=0;j<death_vec.size();j++){
        for(unsigned int i=0;i<player.size();i++){
            if(i!=death_vec[j]&&!player[i].death){
                player[i].points++;
            }
        }
    }
    if(server[1]){
        //
        Pending pending;
        pending.packet << Packet::Points;
        for(unsigned int i=0;i<death_vec.size();i++){
            pending.packet << death_vec[i];
        }
        pending.send_id.push_back(-1);
        mutex.lock();
        packets.push_back(pending);
        mutex.unlock();
    }
}
//
void Game::End_Round(const Config &config,std::vector<Player> &player){
    // Round is always finished so set the var
    round_finished=true;
    // Check if someone won the game
    int points=0; // Numer of points winner has
    round_winner=-1; // The player number
    for(unsigned int i=0;i<player.size();i++){
        // Check if player has the needed points to win
        if(player[i].points>=maxpoints){
            // Check if player has more points then someone other that has more then maxpoints
            if(player[i].points>points){
                points=player[i].points;
                round_winner=i;
            }
            // If two people have the same amount of points revert winner to -1 to let see that there is no winner yet
            else{
                round_winner=-1;
            }
        }
    }
    // Game finshes
    if(round_winner>-1){
        game_finished=true;
        if(update_thread[1]){
            update_thread[2]=true;
        }
        // Server Part :D
        if(server[1]){
            Pending pending;
            pending.packet << Packet::GameEnd << round_winner;
            pending.send_id.push_back(-1);
            mutex.lock();
            packets.push_back(pending);
            mutex.unlock();
        }
    }
    // Game continues, someone has just won a round;
    else{
        for(unsigned int i=0;i<player.size();i++){
            if(player[i].deathframe==0){
                round_winner=i;
            }
        }
        // Server Part :D
        if(server[1]){
            Pending pending;
            pending.packet << Packet::RoundEnd << round_winner;
            pending.send_id.push_back(-1);
            mutex.lock();
            packets.push_back(pending);
            mutex.unlock();
        }
    }
}
//
void Game::Quit(const Config &config,std::vector<Player> &player){
    Shutdown();
    Switch_Mode(Game::Mode::Setup);
}
//
void Game::Shutdown(){
    if(update_thread[1]){
        update_thread[2]=true;
    }
    if(thread.joinable()){
        thread.join();
    }
}
//
void Game::PowerUp_Manager(const Config &config,Game &game,std::vector<Player> &player){
    /*  Powerup List
            0=Fast
            1=Slow
            2=Small
            3=Big
            4=Right Angles
            5=Clear
            6=Invisible
            7=Walls away
            8=More Powerups
            9=Invert Keys
            Total=10
        Powerup Color
            0=Green=self
            1=Red=enemy
            2=Blue=all
            yellow=random?
    */
    // Deletes Powerups that are on the end of their lives
    for(unsigned int i=0;i<game.powerup.size();i++){
        game.powerup[i].time1+=game.elapsed;
        game.powerup[i].time2-=game.elapsed;
        if(game.powerup[i].time2<0.0){
            //
            if(game.server[1]){
                Pending pending;
                pending.packet << Packet::PowerupDelF << game.powerup[i].id;
                pending.send_id.push_back(-1);
                game.mutex.lock();
                game.packets.push_back(pending);
                game.mutex.unlock();
            }
            //
            game.powerup.erase(game.powerup.begin()+i);
        }
    }
    // Remove Powerups for players
    bool del=false;
    for(unsigned int i=0;i<game.player_powerup_effect.size();i++){
        game.player_powerup_effect[i].time-=game.elapsed;
        if(game.player_powerup_effect[i].time<0.0){
            //
            if(game.server[1]){
                Pending pending;
                pending.packet << Packet::PowerupDelP << game.player_powerup_effect[i].id;
                pending.send_id.push_back(-1);
                game.mutex.lock();
                game.packets.push_back(pending);
                game.mutex.unlock();
            }
            //
            game.player_powerup_effect.erase(game.player_powerup_effect.begin()+i);
            del=true;
        }
    }
    // If del then recalculate effects
    if(del){
        for(unsigned int i=0;i<player.size();i++){
            //if(!player[i].death){
                    player[i].Calculate_Powerup_Effect(config,game);
            //}
        }
    }
    // Remove powerup effects that effect whole game
    for(unsigned j=0;j<game.powerup_effect.size();j++){
        game.powerup_effect[j].time-=game.elapsed;
        if(game.powerup_effect[j].time<0.0){
            //
            if(game.powerup_effect[j].type==7){
                game.wallsaway=false;
                if(game.server[1]){
                    Pending pending;
                    pending.packet << Packet::PowerupDelG << game.powerup_effect[j].id;
                    pending.send_id.push_back(-1);
                    game.mutex.lock();
                    game.packets.push_back(pending);
                    game.mutex.unlock();
                }
            }
            else if(game.powerup_effect[j].type==8){
                game.morepowerups=3;
            }
            game.powerup_effect.erase(game.powerup_effect.begin()+j);
        }
    }
    // Then spawn new ones
    // This is done every 0.5 seconds if it succeeds the second if
    game.powerup_spawn_time-=game.elapsed;
    if(game.powerup_spawn_time<0.0){
        game.powerup_spawn_time=config.powerup_spawn_check;
        if(game.morepowerups>0||rand() % (100+1)<config.powerup_spawn_chance){
            //
            game.morepowerups--;
            //
            int powerup_radius=config.powerup_radius;
            bool spawn=false;
            int iter=0; // Debug Variable
            // Making sure powerups doesn't spawn in front or on player
            int X,Y;
            while(!spawn&&iter<500){
                X=config.wallwidth+powerup_radius + rand() % (config.window_width-config.statuswidth-2*config.wallwidth-powerup_radius+1); // Rand x position within walls
                Y=config.wallwidth+powerup_radius + rand() % (config.window_height-2*config.wallwidth-powerup_radius+1);
                spawn=true;
                // Player hit check
                for(unsigned int i=0;i<player.size();i++){
                    if(player[i].death){continue;}
                    // Check 1, Not on player
                    float dx=player[i].x-X;
                    float dy=player[i].y-Y;
                    float radius = config.powerup_radius + player[i].linewidth/2.0;
                    if( (dx*dx) + (dy*dy) < radius*radius ) {
                        spawn=false;
                        break;
                    }
                    // Check 2, not directly in front of player
                    dx=X- (player[i].x+cos(player[i].heading)*config.powerup_safe_d);
                    dy=Y- (player[i].y+sin(player[i].heading)*config.powerup_safe_d);
                    radius = config.powerup_safe_radius;
                    if( (dx*dx) + (dy*dy) < radius*radius ) {
                        spawn=false;
                        break;
                    }
                }
                // Other Powerups hit check
                if(spawn){
                    for(unsigned int j=0;j<game.powerup.size();j++){
                        float dx=game.powerup[j].x-X;
                        float dy=game.powerup[j].y-Y;
                        float radius = config.powerup_radius*2;
                        if( (dx*dx) + (dy*dy) < radius*radius ) {
                            spawn=false;
                        }
                    }
                }
                iter++;
            }
            //
            float D=config.powerup_min_disappear + ( ( rand() % (config.powerup_rand_disappear+1) ) / 1000.0 ); // Disappear Time
            int type = rand() % 10;
            // Impact
            int impact;
            // Green or Red
            if(type==0||type==1||type==2||type==3||type==4){
                impact=rand() % 2;// Green or red / 0 or 1
            }
            // Only Blue
            else if(type==5||type==7||type==8){
                impact=2;
            }
            // Only green
            else if(type==6){
                impact=0;
            }
            // Only Red
            else if(type==9){
                impact=1;
            }
            //
            int id=rand()%50000;
            // Store powerup
            Powerup powerup(X,Y,type,impact,D,id);
            game.powerup.push_back(powerup);
            //
            if(game.server[1]){
                Pending pending;
                pending.packet << Packet::PowerupS << id << X << Y << type << impact << D;
                pending.send_id.push_back(-1);
                game.mutex.lock();
                game.packets.push_back(pending);
                game.mutex.unlock();
            }
            //
        }
    }
    // End with checking if a player hits a powerup
    // i=player
    // j=powerup
    // k=player within effect for enemy's or all
    for(unsigned int i=0;i<player.size();i++){
        if(player[i].death){continue;}
        for(unsigned int j=0;j<game.powerup.size();j++){
            float dx=player[i].x-game.powerup[j].x;
            float dy=player[i].y-game.powerup[j].y;
            float radius = config.powerup_radius + player[i].linewidth/2.0;
            if( (dx*dx) + (dy*dy) < radius*radius ) {
                // Effect Time
                int D=config.powerup_effect_min_disappear + ( ( rand() % (config.powerup_effect_rand_disappear+1) ) / 1000.0 );
                //
                int id=game.powerup[j].id;
                // Effecter
                // Player Effects
                if((game.powerup[j].type!=5&&game.powerup[j].type<7)||game.powerup[j].type==9){
                    game.player_powerup_effect.emplace_back(i,game.powerup[j].type,game.powerup[j].impact,D,id);
                    for(unsigned int k=0;k<player.size();k++){
                        // Calculate powerup effects
                        player[k].Calculate_Powerup_Effect(config,game);
                    }
                }
                // Clear
                else if(game.powerup[j].type==5){
                    for(unsigned int k=0;k<player.size();k++){
                        player[k].line.clear();
                    }
                }
                // Walls Away
                else if(game.powerup[j].type==7){
                    Powerup_Effect effect(game.powerup[j].type,D,id);
                    game.powerup_effect.push_back(effect);
                    game.wallsaway=true;
                }
                // More powerups
                else if(game.powerup[j].type==8){
                    D=config.powerup_more_powerup_delay;
                    Powerup_Effect effect(game.powerup[j].type,D,id);
                    game.powerup_effect.push_back(effect);
                }
                //
                if(game.server[1]){
                    Pending pending;
                    pending.packet << Packet::PowerupHit << i << id;
                    pending.send_id.push_back(-1);
                    game.mutex.lock();
                    game.packets.push_back(pending);
                    game.mutex.unlock();
                }
                //
                // Remove powerup!
                game.powerup.erase(game.powerup.begin()+j);
            } // End within radius check
        }// End powerup loop
    }// End player Hit Check
    //
}
//
void Game::PowerUp_Manager(const Config &config,Game &game){
    for(unsigned int i=0;i<game.powerup.size();i++){
        game.powerup[i].time1+=game.elapsed;
        game.powerup[i].time2-=game.elapsed;
    }
}
//
void Game::Pause(const Config &config, const bool &Pause){
    if(Pause){
        pause=true;
        if(server[1]){
            Pending pending;
            pending.packet << Packet::Pause << true;
            pending.send_id.push_back(-1);
            mutex.lock();
            packets.push_back(pending);
            mutex.unlock();
        }
    }
    else{
        pause=false;
        game_clock.restart();
        if(server[1]){
            packetclock.restart();
            Pending pending;
            pending.packet << Packet::Pause << false;
            pending.send_id.push_back(-1);
            mutex.lock();
            packets.push_back(pending);
            mutex.unlock();
        }
        countdown_int=0;
    }
}
//
