// Contains functions and constructors for the config class
// Needed Header
#include "game.h"
// Constructor
Game::Game(const Config &config): game_pacer(config.game_update_thread_min_time){
    frame=0;
    keychange[0]=-1;
    name_change=-1;
    maxpoints=10;
    mode=Mode::Main_Menu;
    powerup_enabled=true;
    countdown_enabled=false;
    morepowerups=0;
    update_thread[0]=update_thread[1]=update_thread[2]=false;
    server[0]=server[1]=server[2]=false;
    client[0]=client[1]=client[2]=false;
    connected=false;
    server_ip=sf::IpAddress::getLocalAddress().toString();
    countdown_int=0;
    srand (std::time(nullptr));
    refresh_players=false;
    // Set the different powerups
    // Slow
    powerups.emplace_back(Powerup::Type::Slow,Powerup::Impact::Self,100,10,5000);
    powerups.emplace_back(Powerup::Type::Slow,Powerup::Impact::Other,100,10,5000);
    //powerups.emplace_back(Powerup::Type::Slow,Powerup::Impact::All,100,10,5000);
    // Fast
    powerups.emplace_back(Powerup::Type::Fast,Powerup::Impact::Self,100,10,5000);
    powerups.emplace_back(Powerup::Type::Fast,Powerup::Impact::Other,100,10,5000);
    //powerups.emplace_back(Powerup::Type::Fast,Powerup::Impact::All,100,10,5000);
    // Small
    powerups.emplace_back(Powerup::Type::Small,Powerup::Impact::Self,100,10,5000);
    // Big
    powerups.emplace_back(Powerup::Type::Big,Powerup::Impact::Other,100,10,5000);
    // Right Angles
    powerups.emplace_back(Powerup::Type::Right_Angle,Powerup::Impact::Self,50,10,5000);
    powerups.emplace_back(Powerup::Type::Right_Angle,Powerup::Impact::Other,100,10,5000);
    // Clear
    powerups.emplace_back(Powerup::Type::Clear,Powerup::Impact::All,100,10,5000);
    // Invisible
    powerups.emplace_back(Powerup::Type::Invisible,Powerup::Impact::Self,100,10,5000);
    // Walls Away
    powerups.emplace_back(Powerup::Type::Walls_Away,Powerup::Impact::All,100,10,5000);
    // More Powerups
    powerups.emplace_back(Powerup::Type::More_Powerups,Powerup::Impact::All,100,10,5000);
    // Invert Keys
    powerups.emplace_back(Powerup::Type::Invert_Keys,Powerup::Impact::Other,100,10,5000);
    //
    powerups.emplace_back(Powerup::Type::Question_Mark,Powerup::Impact::Other,25,10,5000);
    // Calculate total chance
    total_chance=0;
    for(unsigned int i=0;i<powerups.size();i++){
        total_chance+=powerups[i].spawn_chance;
    }
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
void Game::Initialize(const Config &config, std::vector<Player> &player){
    // Send start game packet to clients
    if(server[1]){
        Pending pending;
        pending.packet << Packet::StartGame;
        pending.send_id.push_back(-1);
        mutex.lock();
        packets.push_back(pending);
        mutex.unlock();
    }
    //
    player_powerup_effect.clear();
    // Make players ready for new game
    for(unsigned int i=0;i<player.size();i++){
        if(!client[1]){
            player[i].New_Game(config,*this);
        }
        player[i].ready=false;
        player[i].points=0;
    }
    round=0;
    packetnumber=0;
    pause=true;
    // Start update thread
    thread = std::thread(&Game::Thread,this,std::cref(config),std::ref(player));
    // At last switch to game screen
    Switch_Mode(Game::Mode::Play);
    //
    New_Round(config,player);
}
//
void Game::Thread(const Config &config,std::vector<Player> &player){
    std::cout << "Game update thread Started" << std::endl;
    update_thread[1]=true;
    while(!update_thread[2]){
        // Only run if not paused and not round finished
        if(countdown_int!=0){
            float elapsed = 3.0 - countdown.getElapsedTime().asSeconds();
            if(elapsed<=0.0){
                countdown_int=0;
                Pause(config,false);
            }
            else if(elapsed <=1.0){
                countdown_int=1;
            }
            else if(elapsed <=2.0){
                countdown_int=2;
            }
        }
        else if( !client[1] && !pause && !round_finished ){
            // Elapsed
            elapsed=game_clock.restart().asSeconds();
            if(elapsed>config.max_dt){
                elapsed=config.max_dt;
                std::cout << "Slow execution!" << std::endl;
            }
            // End Elapsed
            // Powerup
            if(powerup_enabled){PowerUp_Manager(config,player);}
            // Update
            Pending pending;
            if(server[1]){
                pending.send_id.push_back(-1);
                packetnumber++;
                pending.packet << Packet::Update << packetnumber;
            }
            for(unsigned int i=0;i<player.size();i++){
                if(!player[i].death){
                    player[i].Update_Position(config,*this);
                    if(server[1]){
                        pending.packet << i << player[i].x << player[i].y << player[i].heading << (player[i].invisible||player[i].gap[0]<0.0) ;
                    }
                }
            }
            if(server[1]){
                mutex.lock();
                packets.push_back(pending);
                mutex.unlock();
                //
                packettime=packetclock.restart().asSeconds();
            }
            // End Update
            // Hit detection
            Hit_Detector(config,player);
        }
        else if(client[1]&&!round_finished){
            // Check key states and send if changed
            // KeyL changed
            if( (!player[id].left&&sf::Keyboard::isKeyPressed(player[id].keyL)) || (player[id].left&&!sf::Keyboard::isKeyPressed(player[id].keyL)) ){
                player[id].left=sf::Keyboard::isKeyPressed(player[id].keyL);
                // Send package
                Pending pending;
                pending.packet << Packet::KeyL << id << player[id].left;
                mutex.lock();
                packets.push_back(pending);
                mutex.unlock();
            }
            // KeyR changed
            if( (!player[id].right&&sf::Keyboard::isKeyPressed(player[id].keyR)) || (player[id].right&&!sf::Keyboard::isKeyPressed(player[id].keyR)) ){
                player[id].right=sf::Keyboard::isKeyPressed(player[id].keyR);
                // Send package
                Pending pending;
                pending.packet << Packet::KeyR << id << player[id].right;
                mutex.lock();
                packets.push_back(pending);
                mutex.unlock();
            }
            // Powerup
            if(!pause){
                elapsed=game_clock.restart().asSeconds();
                PowerUp_Manager(config);
            };
        }
        // Pacer
        game_pacer.Pace();
        frame++;
    }
    // Shutdown
    update_thread[0]=false;
    update_thread[1]=false;
    update_thread[2]=false;
    std::cout << "Game update thread Ended" << std::endl;
}
//
void Game::New_Round(const Config &config,std::vector<Player> &player){
    player_powerup_effect.clear();
    // Random position for the players and heading
    for(unsigned int i=0;i<player.size();i++){
        player[i].New_Round(config,*this);
        player[i].place=i;
    }
    // Send data
    if(server[1]){
        // Send data to clients
        Pending pending;
        pending.packet << Packet::NewRound;
        for(unsigned int i=0;i<player.size();i++){
            pending.packet << i << player[i].x << player[i].y << player[i].heading;
        }
        pending.send_id.push_back(-1);
        mutex.lock();
        packets.push_back(pending);
        mutex.unlock();
    }
    // Reset Some vars
    deathcount=0;
    round_finished=false;
    game_finished=false;
    end_message_set=false;
    pause=true; // Pause_Game(config,game,window,true);
    round++;
    powerup_field.clear();
    powerup_effect.clear();
    wallsaway=false;
    morepowerups=0;
    powerup_spawn_time=config.powerup_spawn_delay;
    // Ready 'm up!
    if(server[1]){
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
        countdown.restart();
        countdown_int=3;
        Pending pending;
        pending.send_id.push_back(-1);
        pending.packet << Packet::Countdown;
        mutex.lock();
        packets.push_back(pending);
        mutex.unlock();
    }
    else if(!client[1]&&countdown_enabled){
        countdown.restart();
        countdown_int=3;
    }
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
            // Only when not gapping or invisible
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
                    else if(j==i){
                        // Delay should be depending on speed and line width
                        unsigned int delay=config.shift;
                        // Linewidth scaling
                        if(player[j].linewidth>config.linewidth){
                            delay*=player[j].linewidth/config.linewidth;
                        }
                        // Right Angles
                        if(player[j].rightangle){
                            delay*=1.5;
                        }
                        // Speed scaling
                        if(player[j].shift<config.shift){
                            delay*=config.shift/player[j].shift;
                        }
                        // Note that it is possible that having a very slow speed and then accelerating fast could maybe also trigger a hit
                        // This is to check if the line is long enough. I think I could better add this to player::powerup_effect.
                        if(player[j].line.getVertexCount()>delay){
                            for(unsigned int k=0;k<player[j].line.getVertexCount()-delay;k++){
                                diffx=x-player[j].line[k].position.x;
                                diffy=y-player[j].line[k].position.y;
                                if( (diffx*diffx) + (diffy*diffy) < player[i].linewidth*player[i].linewidth/4 ){
                                    death_vec.emplace_back(i);
                                    break;
                                }
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
void Game::Player_Death(std::vector<Player> &player,const std::vector<unsigned int> &death_vec){
    // Give points
    for(unsigned int j=0;j<death_vec.size();j++){
        for(unsigned int i=0;i<player.size();i++){
            if(i!=death_vec[j]&&!player[i].death){
                player[i].points++;
            }
        }
    }
    // Server things
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
    // Do these actions for all dead people
    for(unsigned int i=0;i<death_vec.size();i++){
        deathcount++;
        player[death_vec[i]].death=true;
        player[death_vec[i]].deathframe=frame;
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
void Game::PowerUp_Manager(const Config &config,std::vector<Player> &player){
    // Deletes Powerups that are on the end of their lives
    for(unsigned int i=0;i<powerup_field.size();i++){
        powerup_field[i].time1+=elapsed;
        powerup_field[i].time2-=elapsed;
        if(powerup_field[i].time2<0.0){
            //
            if(server[1]){
                Pending pending;
                pending.packet << Packet::PowerupDelF << powerup_field[i].id;
                pending.send_id.push_back(-1);
                mutex.lock();
                packets.push_back(pending);
                mutex.unlock();
            }
            //
            powerup_field.erase(powerup_field.begin()+i);
        }
    }
    // Remove Powerups for players
    bool del=false;
    for(unsigned int i=0;i<player_powerup_effect.size();i++){
        player_powerup_effect[i].time-=elapsed;
        if(player_powerup_effect[i].time<0.0){
            //
            if(server[1]){
                Pending pending;
                pending.packet << Packet::PowerupDelP << player_powerup_effect[i].id;
                pending.send_id.push_back(-1);
                mutex.lock();
                packets.push_back(pending);
                mutex.unlock();
            }
            //
            player_powerup_effect.erase(player_powerup_effect.begin()+i);
            del=true;
        }
    }
    // If del then recalculate effects
    if(del){
        for(unsigned int i=0;i<player.size();i++){
                player[i].Calculate_Powerup_Effect(config,*this);
        }
    }
    // Remove powerup effects that effect whole game
    for(unsigned j=0;j<powerup_effect.size();j++){
        powerup_effect[j].time-=elapsed;
        if(powerup_effect[j].time<0.0){
            //
            if(powerup_effect[j].type==Powerup::Type::Walls_Away){
                wallsaway=false;
                if(server[1]){
                    Pending pending;
                    pending.packet << Packet::PowerupDelG << powerup_effect[j].id;
                    pending.send_id.push_back(-1);
                    mutex.lock();
                    packets.push_back(pending);
                    mutex.unlock();
                }
            }
            else if(powerup_effect[j].type==Powerup::Type::More_Powerups){
                morepowerups=3;
            }
            powerup_effect.erase(powerup_effect.begin()+j);
        }
    }
    // Then spawn new ones
    // This is done every 0.5 seconds if it succeeds the second if
    powerup_spawn_time-=elapsed;
    if(powerup_spawn_time<0.0){
        powerup_spawn_time=config.powerup_spawn_check;
        if(morepowerups>0||rand() % (100+1)<config.powerup_spawn_chance){
            //
            morepowerups--;
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
                // Other Powerups hit check; I should remove this.
                /*if(spawn){
                    for(unsigned int j=0;j<powerup.size();j++){
                        float dx=powerup[j].x-X;
                        float dy=powerup[j].y-Y;
                        float radius = config.powerup_radius*2;
                        if( (dx*dx) + (dy*dy) < radius*radius ) {
                            spawn=false;
                        }
                    }
                }*/
                iter++;
            }
            // Choose a powerup
            Powerup::Type type;
            Powerup::Impact impact;
            Choose_PowerUp(type,impact);
            // Calculate disappear time
            float D=config.powerup_min_disappear + ( ( rand() % (config.powerup_rand_disappear+1) ) / 1000.0 ); // Disappear Time
            // Set ID so it easier to identify
            int id=std::time(nullptr);
            // Store powerup
            powerup_field.emplace_back(X,Y,type,impact,D,id);
            //
            if(server[1]){
                Pending pending;
                pending.packet << Packet::PowerupS << id << X << Y << type << impact << D;
                pending.send_id.push_back(-1);
                mutex.lock();
                packets.push_back(pending);
                mutex.unlock();
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
        for(unsigned int j=0;j<powerup_field.size();j++){
            float dx=player[i].x-powerup_field[j].x;
            float dy=player[i].y-powerup_field[j].y;
            float radius = config.powerup_radius + player[i].linewidth/2.0;
            if( (dx*dx) + (dy*dy) < radius*radius ) {
                // Effect Time
                int D=config.powerup_effect_min_disappear + ( ( rand() % (config.powerup_effect_rand_disappear+1) ) / 1000.0 );
                //
                int id=powerup_field[j].id;
                // Question Mark
                while(powerup_field[j].type==Powerup::Type::Question_Mark){
                    Choose_PowerUp(powerup_field[j].type,powerup_field[j].impact);
                }
                // Effecter
                // Player Effects
                switch (powerup_field[j].type){
                    case Powerup::Type::Slow:
                    case Powerup::Type::Fast:
                    case Powerup::Type::Small:
                    case Powerup::Type::Big:
                    case Powerup::Type::Right_Angle:
                    case Powerup::Type::Invisible:
                    case Powerup::Type::Invert_Keys:
                        player_powerup_effect.emplace_back(i,powerup_field[j].type,powerup_field[j].impact,D,id);
                        for(unsigned int k=0;k<player.size();k++){
                            // Calculate powerup effects
                            player[k].Calculate_Powerup_Effect(config,*this);
                        }
                        break;
                    case Powerup::Type::Clear:
                        for(unsigned int k=0;k<player.size();k++){
                            player[k].line.clear();
                        }
                        break;
                    case Powerup::Type::Walls_Away:
                        powerup_effect.emplace_back(Powerup::Type::Walls_Away,D,id);
                        wallsaway=true;
                        break;
                    case Powerup::Type::More_Powerups:
                        powerup_effect.emplace_back(Powerup::Type::More_Powerups,config.powerup_more_powerup_delay,id);
                        break;
                    default:
                        break;
                }
                //
                if(server[1]){
                    Pending pending;
                    pending.packet << Packet::PowerupHit << i << id;
                    pending.send_id.push_back(-1);
                    mutex.lock();
                    packets.push_back(pending);
                    mutex.unlock();
                }
                //
                // Remove powerup!
                powerup_field.erase(powerup_field.begin()+j);
            } // End within radius check
        }// End powerup loop
    }// End player Hit Check
    //
}
//
void Game::PowerUp_Manager(const Config &config){
    for(unsigned int i=0;i<powerup_field.size();i++){
        powerup_field[i].time1+=elapsed;
        powerup_field[i].time2-=elapsed;
    }
}
//
void Game::Choose_PowerUp(Powerup::Type &type, Powerup::Impact &impact){
    int random=rand() % total_chance;
    int low=0;
    for(unsigned int i=0;i<powerups.size();i++){
        if( random>=low && random<(low+powerups[i].spawn_chance) ){
            type=powerups[i].type;
            impact=powerups[i].impact;
            return;
        }
        else{
            low+=powerups[i].spawn_chance;
        }
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
