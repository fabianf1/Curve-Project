// Contains game function
#include "../curve.h"
//
void Initialize_Game(const Config &config,Game &game,Player player[]){
    // Check if atleast two players are enabled and have their buttons set
    int players=0;
    for(int i=0;i<MAX_PLAYERS;i++){
        if(!player[i].enabled||player[i].keyL==sf::Keyboard::Unknown||player[i].keyR==sf::Keyboard::Unknown){continue;}
        players++;
        // Reset Vars
        player[i].points=0;
        player[i].place=i;
        player[i].Calculate_Powerup_Effect(config,game);
    }
    // If atleast two players start game init
    if(players>=2){
        // Set the seed
        sf::Time time=game.randclock.getElapsedTime();
        srand (time.asSeconds()+time.asMilliseconds());
        // Set players
        game.players=players;
        game.round=0;
        Initialize_New_Round(config,game,player);
        game.mode=Game::Mode::Play;
        // Message Main thread to start update thread
        game.update_thread[0]=true;
    }
}
//
void Initialize_Game_Client(const Config &config,Game &game,Player player[]){
    game.round=0;
    Initialize_New_Round_Client(config,game,player);
    for(int i=0;i<MAX_PLAYERS;i++){
        player[i].points=0;
        player[i].left=player[i].right=false; // Button set?
        player[i].place=i;
    }
    //
    game.update_thread[0]=true;
    game.mode=Game::Mode::Play_MP;

}
//
void Initialize_New_Round(const Config &config,Game &game,Player player[]){
    game.player_powerup_effect.clear();
    // Random position for the players and heading
    for(int i=0;i<MAX_PLAYERS;i++){
        if(!player[i].enabled){continue;}
        player[i].New_Round(config,game);
    }
    // Reset Some vars
    game.deathcount=0;
    game.round_finished=false;
    game.game_finished=false;
    game.pause=true; // Pause_Game(config,game,window,true);
    game.round++;
    game.powerup.clear();
    game.powerup_effect.clear();
    game.wallsaway=false;
    game.morepowerups=0;
    game.powerup_spawn_time=config.powerup_spawn_delay;
}
//
void Initialize_New_Round_Server(const Config &config,Game &game,Player player[]){
    game.player_powerup_effect.clear();
    for(int i=0;i<MAX_PLAYERS;i++){
        if(!player[i].enabled){continue;}
        player[i].New_Round_Server(config,game,i);
    }
    // Reset Some vars
    game.deathcount=0;
    game.round_finished=false;
    game.game_finished=false;
    game.pause=true; // Pause_Game(config,game,window,true);
    game.round++;
    game.powerup.clear();
    game.powerup_effect.clear();
    game.wallsaway=false;
    game.morepowerups=0;
    game.powerup_spawn_time=config.powerup_spawn_delay;
    game.send=true;
    game.packetnumber=0;
    game.countdown.restart();
    game.countdown_int=3;
    // Set countdown
    /*game.countdown=3;
    Pending pending;
    pending.send_id.push_back(-1);
    pending.packet << Packet::Countdown << game.countdown;
    game.mutex.lock();
    game.pending.push_back(pending);
    game.mutex.unlock();*/
    // Not needed: Let client auto set countdown to three

}
//
void Initialize_New_Round_Client(const Config &config,Game &game,Player player[]){
    game.player_powerup_effect.clear();
    for(int i=0;i<MAX_PLAYERS;i++){
        player[i].ready=false;
        player[i].death=false;
        player[i].Calculate_Powerup_Effect(config,game);
    }
    // Reset Some vars
    game.round_finished=false;
    game.game_finished=false;
    game.pause=true;
    game.round++;
    game.powerup.clear();
    game.powerup_effect.clear();
    game.wallsaway=false;
    game.packetnumber=0;
    game.countdown_int=3;
}
//
void Start_Game_Update(const Config &config,Game &game,Player player[]){
    game.update_thread[0]=false;
    if(game.server[1]){
        game.threads.emplace_back(Game_Update_Thread_Server,std::cref(config),std::ref(game),player);
    }
    else if(game.client[1]){
        game.threads.emplace_back(Game_Update_Thread_Client,std::cref(config),std::ref(game),player);
    }
    else{
        game.threads.emplace_back(Game_Update_Thread,std::cref(config),std::ref(game),player);
    }
}
//
void Game_Update_Thread(const Config &config,Game &game,Player player[]){
    std::cout << "Game update thread Started" << std::endl;
    game.update_thread[1]=true;
    game.game_update_thread_clock.restart();
    while(!game.update_thread[2]){
        // Only run if not paused and not round finished
        if(!game.pause&&!game.round_finished){
            // Elapsed
            float elapsed=game.game_clock.restart().asSeconds();
            if(elapsed>config.max_dt){
                game.elapsed=config.max_dt;
            }
            else{
                game.elapsed=elapsed;
            }
            // End Elapsed
            // Powerup
            if(game.powerup_enabled){PowerUp_Manager(config,game,player);}
            // Update
            for(int i=0;i<MAX_PLAYERS;i++){
                if(!player[i].enabled||player[i].death){continue;}// Don't process disabled and death players
                player[i].Update_Position_Local(config,game); // Update position if playing
            }
            // End Update
            // Hit detection
            Hit_Detector(config,game,player);
        } // End only run
        Sleep(config.game_update_thread_min_time,game.game_update_thread_clock);
    }
    // Shutdown
    game.update_thread[0]=false;
    game.update_thread[1]=false;
    game.update_thread[2]=false;
    std::cout << "Game update thread Ended" << std::endl;
}
//
void Game_Update_Thread_Server(const Config &config,Game &game,Player player[]){
    // Update Player Positions!
    std::cout << "Game update thread Started" << std::endl;
    game.update_thread[1]=true;
    game.game_update_thread_clock.restart();
    while(!game.update_thread[2]){
        // Countdown handling
        if(game.countdown_int!=0){
            float elapsed = 3.0 - game.countdown.getElapsedTime().asSeconds();
            if(elapsed<=0.0){
                game.countdown_int=0;
                Pause_Game(config,game,false);
            }
            else if(elapsed <=1.0){
                game.countdown_int=1;
                Pending pending;
                pending.send_id.push_back(-1);
                pending.packet << Packet::Countdown << game.countdown_int;
                game.mutex.lock();
                game.pending.push_back(pending);
                game.mutex.unlock();
            }
            else if(elapsed <=2.0){
                game.countdown_int=2;
                Pending pending;
                pending.send_id.push_back(-1);
                pending.packet << Packet::Countdown << game.countdown_int;
                game.mutex.lock();
                game.pending.push_back(pending);
                game.mutex.unlock();
            }
        }
        // Only run if not paused and not round finished
        if(!game.pause&&!game.round_finished){
            // Elapsed
            float elapsed=game.game_clock.restart().asSeconds();
            if(elapsed>config.max_dt){
                game.elapsed=config.max_dt;
            }
            else{
                game.elapsed=elapsed;
            }
            // End Elapsed
            // Powerup
            if(game.powerup_enabled){PowerUp_Manager(config,game,player);}
            // Update
            player[0].left=sf::Keyboard::isKeyPressed(player[game.id].keyL);
            player[0].right=sf::Keyboard::isKeyPressed(player[game.id].keyR);
            // If Sending a Package
            if(game.send){
                Pending pending;
                pending.send_id.push_back(-1);
                game.packetnumber++;
                pending.packet << Packet::Update << game.packetnumber;
                for(int i=0;i<MAX_PLAYERS;i++){
                    if(player[i].enabled&& (!player[i].death || (player[i].death&&player[i].sendlast) ) ){
                        if(!player[i].death){
                            player[i].Update_Position(config,game);
                        }
                        else{
                            player[i].sendlast=false;
                        }
                        // Operations for the package
                        // Big Package if noline is switching and last position was not sent
                        if( (!player[i].noline && (player[i].invisible||player[i].gap[0]<0.0) ) || (player[i].noline&& !player[i].invisible && player[i].gap[0]>0.0) ){
                            player[i].noline=!player[i].noline;
                            pending.packet << i << player[i].xOLD << player[i].yOLD << player[i].hOLD << !player[i].noline << i << player[i].x << player[i].y << player[i].h << player[i].noline;
                        }
                        // Normal Package otherwise
                        else{
                            pending.packet << i << player[i].x << player[i].y << player[i].h << player[i].noline;
                        }
                    }
                }
                // Send Package
                game.mutex.lock();
                game.pending.push_back(pending);
                game.mutex.unlock();
                game.send=false;
                //
                game.packettime=game.packetclock.restart().asSeconds();
            }
            // If not, do normal
            else{
                for(int i=0;i<MAX_PLAYERS;i++){
                    if(!player[i].enabled||player[i].death){continue;}// Don't process disabled and/or death players
                    player[i].Update_Position(config,game);
                    if( (!player[i].noline && (player[i].invisible||player[i].gap[0]<0.0) ) || (player[i].noline&& !player[i].invisible && player[i].gap[0]>0.0) ){
                        player[i].noline=!player[i].noline;
                    }
                }
                //
                game.send=true;
            }
            // End Update
            // Hit detection
            Hit_Detector(config,game,player);
        } // End only run
        Sleep(config.game_update_thread_min_time,game.game_update_thread_clock);
    }
    // Shutdown
    game.update_thread[0]=false;
    game.update_thread[1]=false;
    game.update_thread[2]=false;
    std::cout << "Game update thread Ended" << std::endl;
}
//
void Game_Update_Thread_Client(const Config &config,Game &game,Player player[]){
    std::cout << "Game update thread Started" << std::endl;
    game.update_thread[1]=true;
    // Some vars
    game.game_update_thread_clock.restart();
    while(!game.update_thread[2]){
        // Check key states and send if changed
        // KeyL changed
        if( (!player[game.id].left&&sf::Keyboard::isKeyPressed(player[game.id].keyL)) || (player[game.id].left&&!sf::Keyboard::isKeyPressed(player[game.id].keyL)) ){
            player[game.id].left=!player[game.id].left;
            // Send package
            Pending pending;
            pending.packet << Packet::KeyL << game.id << player[game.id].left;
            game.mutex.lock();
            game.pending.push_back(pending);
            game.mutex.unlock();
        }
        // KeyR changed
        if( (!player[game.id].right&&sf::Keyboard::isKeyPressed(player[game.id].keyR)) || (player[game.id].right&&!sf::Keyboard::isKeyPressed(player[game.id].keyR)) ){
            player[game.id].right=!player[game.id].right;
            // Send package
            Pending pending;
            pending.packet << Packet::KeyR << game.id << player[game.id].right;
            game.mutex.lock();
            game.pending.push_back(pending);
            game.mutex.unlock();
        }
        // Powerup
        if(!game.pause){
            game.elapsed=game.game_clock.restart().asSeconds();
            PowerUp_Manager_Client(config,game);
        };
        Sleep(config.game_update_thread_min_time,game.game_update_thread_clock);
    }
    // Shutdown
    game.update_thread[0]=false;
    game.update_thread[1]=false;
    game.update_thread[2]=false;
    std::cout << "Game update thread Ended" << std::endl;
}
//
void Hit_Detector(const Config &config,Game &game,Player player[]){
    // Initializer vector to store death people
    std::vector<int> death_vec;
    // Do the magic
    for(int i=0;i<MAX_PLAYERS;i++){
        if(!player[i].enabled||player[i].death){continue;}
        // Player Position
        float x=player[i].x;
        float y=player[i].y;
        // Out of bounds check, Only if there are walls
        if(!game.wallsaway&& (x-player[i].linewidth/2<config.wallwidth||x+player[i].linewidth/2>config.window_width-config.statuswidth-config.wallwidth||y-player[i].linewidth/2<config.wallwidth||y+player[i].linewidth/2>config.window_height-config.wallwidth) ){
            death_vec.emplace_back(i);
        }
        else{
            // Line hit check
            // Only when not gapping
            if(player[i].gap[0]>0.0&&!player[i].invisible){
                float diffx,diffy;
                //
                for(int j=0;j<MAX_PLAYERS;j++){
                    if(!player[j].enabled){continue;}
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
        Player_Death(game,player,death_vec);
        // Stop round if less then two players
        if(game.deathcount>=game.players+1){ // For debugging purposes
            std::cout << "Multiple deaths!" << std::endl;
            End_Round(config,game,player);
        }
        else if(game.deathcount>=game.players-1){
            // End Round function
            End_Round(config,game,player);
        }
    }
}
//
void Player_Death(Game &game,Player player[],std::vector<int> &death_vec){
    if(game.server[1]){
            Add_Points_Server(game,player,death_vec);
            // Make sure last position is send
            if(game.send){
                for(unsigned int i=0;i<death_vec.size();i++){
                    player[death_vec[i]].sendlast=true;
                }
            }
    }
    else{Add_Points(player,death_vec);}
    // Do these actions for all dead people
    for(unsigned int i=0;i<death_vec.size();i++){
        game.deathcount++;
        player[death_vec[i]].death=true;
        player[death_vec[i]].deathframe=game.frame;
    }
}
//
void Add_Points(Player player[], std::vector<int> &death_vec){
    for(unsigned int j=0;j<death_vec.size();j++){
        for(int i=0;i<MAX_PLAYERS;i++){
            if(i!=death_vec[j]&&player[i].enabled&&!player[i].death){
                player[i].points++;
            }
        }
    }
}
//
void Add_Points_Server(Game &game,Player player[], std::vector<int> &death_vec){
    Add_Points(player,death_vec);
    //
    Pending pending;
    pending.packet << Packet::Points;
    for(unsigned int i=0;i<death_vec.size();i++){
        pending.packet << death_vec[i];
    }
    pending.send_id.push_back(-1);
    game.mutex.lock();
    game.pending.push_back(pending);
    game.mutex.unlock();
}
//
void End_Round(const Config &config,Game &game,Player player[]){
    // Round is always finished so set the var
    game.round_finished=true;
    // Check if someone won the game
    int points=0; // Numer of points winner has
    game.round_winner=-1; // The player number
    for(int i=0;i<MAX_PLAYERS;i++){
        if(!player[i].enabled){continue;}
        // Check if player has the needed points to win
        if(player[i].points>=game.maxpoints){
            // Check if player has more points then someone other that has more then maxpoints
            if(player[i].points>points){
                points=player[i].points;
                game.round_winner=i;
            }
            // If two people have the same amount of points revert winner to -1 to let see that there is no winner yet
            else{
                game.round_winner=-1;
            }
        }
    }
    // Game finshes
    if(game.round_winner>-1){
        game.game_finished=true;
        if(game.update_thread[1]){
            game.update_thread[2]=true;
        }
        // Server Part :D
        if(game.server[1]){
            Pending pending;
            pending.packet << Packet::GameEnd << game.round_winner;
            pending.send_id.push_back(-1);
            game.mutex.lock();
            game.pending.push_back(pending);
            game.mutex.unlock();
        }
    }
    // Game continues, someone has just won a round;
    else{
        for(int i=0;i<MAX_PLAYERS;i++){
            if(!player[i].enabled){continue;}
            if(player[i].deathframe==0){
                game.round_winner=i;
            }
        }
        // Server Part :D
        if(game.server[1]){
            Pending pending;
            pending.packet << Packet::RoundEnd << game.round_winner;
            pending.send_id.push_back(-1);
            game.mutex.lock();
            game.pending.push_back(pending);
            game.mutex.unlock();
        }
    }
}
//
void PowerUp_Manager(const Config &config,Game &game,Player player[]){
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
                game.pending.push_back(pending);
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
                game.pending.push_back(pending);
                game.mutex.unlock();
            }
            //
            game.player_powerup_effect.erase(game.player_powerup_effect.begin()+i);
            del=true;
        }
    }
    // If del then recalculate effects
    if(del){
        for(int i=0;i<MAX_PLAYERS;i++){
            if(player[i].enabled&&!player[i].death){player[i].Calculate_Powerup_Effect(config,game);}
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
                    game.pending.push_back(pending);
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
                for(unsigned int i=0;i<MAX_PLAYERS;i++){
                    if(!player[i].enabled||player[i].death){continue;}
                    // Check 1, Not on player
                    float dx=player[i].x-X;
                    float dy=player[i].y-Y;
                    float radius = config.powerup_radius + player[i].linewidth/2.0;
                    if( (dx*dx) + (dy*dy) < radius*radius ) {
                        spawn=false;
                        break;
                    }
                    // Check 2, not directly in front of player
                    dx=X- (player[i].x+cos(player[i].h)*config.powerup_safe_d);
                    dy=Y- (player[i].y+sin(player[i].h)*config.powerup_safe_d);
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
                //std::cout<<iter<<std::endl;
            }
            //
            float D=config.powerup_min_disappear + ( ( rand() % (config.powerup_rand_disappear+1) ) / 1000.0 ); // Disappear Time
            int type = rand() % 10;
            //int type = 3;
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
                game.pending.push_back(pending);
                game.mutex.unlock();
            }
            //
        }
    }
    // End with checking if a player hits a powerup
    // i=player
    // j=powerup
    // k=player within effect for enemy's or all
    for(unsigned int i=0;i<MAX_PLAYERS;i++){
        if(!player[i].enabled||player[i].death){continue;}
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
                    for(int k=0;k<MAX_PLAYERS;k++){
                        // Calculate powerup effects
                        player[k].Calculate_Powerup_Effect(config,game);
                    }
                }
                // Clear
                else if(game.powerup[j].type==5){
                    for(unsigned int k=0;k<MAX_PLAYERS;k++){
                        if(!player[k].enabled){continue;}
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
                    game.pending.push_back(pending);
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
void PowerUp_Manager_Client(const Config &config,Game &game){
    for(unsigned int i=0;i<game.powerup.size();i++){
        game.powerup[i].time1+=game.elapsed;
        game.powerup[i].time2-=game.elapsed;
    }
}
//


/* Backup of last working version for points
void Hit_Detector(const Config &config,Game &game,Player player[]){
    for(int i=0;i<MAX_PLAYERS;i++){
        if(!player[i].enabled||player[i].death){continue;}
        // Player Position
        float x=player[i].x;
        float y=player[i].y;
        // Out of bounds check, Only if there are walls
        if(!game.wallsaway&& (x-player[i].linewidth/2<config.wallwidth||x+player[i].linewidth/2>config.window_width-config.statuswidth-config.wallwidth||y-player[i].linewidth/2<config.wallwidth||y+player[i].linewidth/2>config.window_height-config.wallwidth) ){
            Player_Death(game,player,i);
        }
        else{
            // Line hit check
            // Only when not gapping
            if(player[i].gap[0]>0.0&&!player[i].invisible){
                float diffx,diffy;
                //
                for(int j=0;j<MAX_PLAYERS;j++){
                    if(!player[j].enabled){continue;}
                    bool hit=false;
                    // Other Player
                    if(j!=i){
                        // Normal Detection
                        for(unsigned int k=0;k<player[j].line.getVertexCount();k++){
                            diffx=x-player[j].line[k].position.x;
                            diffy=y-player[j].line[k].position.y;
                            if( (diffx*diffx) + (diffy*diffy) < player[i].linewidth*player[i].linewidth/4 ){
                                Player_Death(game,player,i);
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
                                Player_Death(game,player,i);
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
                                Player_Death(game,player,i);
                                break;
                            }
                        }
                    } // End own line hit check
                } // End for loop
            } // End frame>gap
        }// End else
    } // End for loop
    // Stop round if less then two players
    if(game.deathcount>=game.players+1){ // For debugging purposes
        std::cout << "Multiple deaths!" << std::endl;
        End_Round(config,game,player);
    }
    else if(game.deathcount>=game.players-1){
        // End Round function
        End_Round(config,game,player);
    }
}
//
void Player_Death(Game &game,Player player[],const int &i){
    if(game.server[1]){
            Add_Points_Server(game,player,i);
            // Make sure last position is send
            if(game.send){
                player[i].sendlast=true;
            }
    }
    else{Add_Points(player,i);}
    game.deathcount++;
    player[i].death=true;
    player[i].deathframe=game.frame;
}
//
void Add_Points(Player player[], int P){
    for(int i=0;i<MAX_PLAYERS;i++){
        if(i!=P&&player[i].enabled&&!player[i].death){
            player[i].points++;
        }
    }
}
//
void Add_Points_Server(Game &game,Player player[], int P){
    Add_Points(player,P);
    //
    Pending pending;
    pending.packet << Packet::Points << P;
    pending.send_id.push_back(-1);
    game.mutex.lock();
    game.pending.push_back(pending);
    game.mutex.unlock();
}
*/
