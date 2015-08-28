// Contains functions and constructors for the config class
// Needed Header
#include "Game.h"
// Constructor
Game::Game(const Config &config): gamePacer(config.gameUpdateThreadMinRate), randGenerator(std::time(nullptr)){
    frame=0;
    keyChange[0]=-1;
    nameChange=-1;
    removedPlayer=-1;
    maxPoints=10;
    mode=Mode::mainMenu;
    powerupEnabled=true;
    countdownEnabled=false;
    multiplePlayersEnabled=false;
    morepowerups=0;
    updateThread[0]=updateThread[1]=updateThread[2]=false;
    server[0]=server[1]=server[2]=false;
    client[0]=client[1]=client[2]=false;
    countdownInt=0;
    refreshPlayers=false;
    refreshOptions=false;
    //
    initializePowerups(config);
    // Random
    srand (std::time(nullptr));
    randPowerup.param(std::uniform_int_distribution<int> (0,totalChance).param());
    randSpawn.param(std::uniform_int_distribution<int> (0,100).param());
}
// Functions
void Game::switchMode(const Game::Mode &Mode){
    modeMutex.lock();
    mode=Mode;
    modeMutex.unlock();
}
//
void Game::initialize(const Config &config, std::vector<Player> &player){
    // Send start game packet to clients
    if(server[1]){
        Pending pending;
        pending.packet << Packet::StartGame;
        queuePacket(pending);
    }
    //
    playerPowerupEffect.clear();
    // Make players ready for new game
    for(unsigned int i=0;i<player.size();i++){
        player[i].New_Game();
    }
    round=0;
    packetNumber=0;
    paused=true;
    // start update thread
    gameThread = std::thread(&Game::thread,this,std::cref(config),std::ref(player));
    // At last toggle to game screen
    switchMode(Game::Mode::Play);
    //
    if(!client[1]){
        newRound(config,player);
    }
}
//
void Game::initializePowerups(const Config &config){
    // Slow
    powerups.emplace_back(Powerup::Type::Slow,Powerup::Impact::Self,100,5,2500);
    powerups.emplace_back(Powerup::Type::Slow,Powerup::Impact::Other,100,5,5000);
    // Fast
    powerups.emplace_back(Powerup::Type::Fast,Powerup::Impact::Self,100,5,2500);
    powerups.emplace_back(Powerup::Type::Fast,Powerup::Impact::Other,100,5,5000);
    // Small
    powerups.emplace_back(Powerup::Type::Small,Powerup::Impact::Self,100,5,5000);
    // Big
    powerups.emplace_back(Powerup::Type::Big,Powerup::Impact::Other,100,5,5000);
    // Right Angles
    powerups.emplace_back(Powerup::Type::RightAngle,Powerup::Impact::Self,50,5,5000);
    powerups.emplace_back(Powerup::Type::RightAngle,Powerup::Impact::Other,100,5,5000);
    // Clear
    powerups.emplace_back(Powerup::Type::Clear,Powerup::Impact::All,100,0,0);
    // Invisible
    powerups.emplace_back(Powerup::Type::Invisible,Powerup::Impact::Self,100,5,5000);
    // Walls Away
    powerups.emplace_back(Powerup::Type::WallsAway,Powerup::Impact::All,100,10,0);
    // More Powerups
    powerups.emplace_back(Powerup::Type::MorePowerups,Powerup::Impact::All,100,config.powerupMorePowerupDelay,0);
    // Invert Keys
    powerups.emplace_back(Powerup::Type::InvertKeys,Powerup::Impact::Other,100,5,5000);
    // ?
    powerups.emplace_back(Powerup::Type::QuestionMark,Powerup::Impact::Other,25,0,0);
    // Darkness
    powerups.emplace_back(Powerup::Type::Darkness,Powerup::Impact::All,25,7.5,5000);
    // Gap
    powerups.emplace_back(Powerup::Type::Gap,Powerup::Impact::Other,75,5,5000);
    // Bomb
    powerups.emplace_back(Powerup::Type::Bomb,Powerup::Impact::All,50,0,0);
    // Sine
    powerups.emplace_back(Powerup::Type::Sine,Powerup::Impact::Self,50,(1/config.sineFrequency)*5,0);
    powerups.emplace_back(Powerup::Type::Sine,Powerup::Impact::Other,75,(1/config.sineFrequency)*5,0);
    // Glitch
    powerups.emplace_back(Powerup::Type::Glitch,Powerup::Impact::All,15,5,5000);
    powerups.emplace_back(Powerup::Type::Glitch,Powerup::Impact::Other,10,5,5000);
    // Radius
    powerups.emplace_back(Powerup::Type::Radius,Powerup::Impact::All,25,5,0);
    // NoTurtle
    powerups.emplace_back(Powerup::Type::NoTurtle,Powerup::Impact::All,25,5,0);
    powerups.emplace_back(Powerup::Type::NoTurtle,Powerup::Impact::Other,40,5,0);
    // Multiplier
    powerups.emplace_back(Powerup::Type::Multiplier,Powerup::Impact::Self,20,5,0);
    // Calculate total chance
    totalChance=0;
    for(unsigned int i=0;i<powerups.size();i++){
        totalChance+=powerups[i].spawnChance;
    }
}
//
void Game::thread(const Config &config,std::vector<Player> &player){
    std::cout << "Game update thread Started" << std::endl;
    updateThread[1]=true;
    while(!updateThread[2]){
        // Only run if not paused and not round finished
        // I should remove the integer and only work with the float.
        if(countdownInt!=0){
            float elapsed = 3.0 - countdownClock.getElapsedTime().asSeconds();
            if(elapsed<=0.0){
                countdownInt=0;
                pause(false);
            }
            else if(elapsed <=1.0){
                countdownInt=1;
            }
            else if(elapsed <=2.0){
                countdownInt=2;
            }
        }
        else if( !client[1] && !paused && !roundFinished ){
            // Get elapsed time since last iteration. If the time is too long it will be set to a fixed value.
            elapsed=gameClock.restart().asSeconds();
            if(elapsed>config.maxElapsed){
                elapsed=config.maxElapsed;
                std::cout << "Slow execution!" << std::endl;
            }
            // Powerup
            if(powerupEnabled){
                powerUpManager(config,player);
            }
            // Update
            Pending pending;
            if(server[1]){
                packetNumber++;
                pending.packet << Packet::Update << packetNumber;
            }
            for(unsigned int i=0;i<player.size();i++){
                if(!player[i].death){
                    player[i].updatePosition(config,*this);
                    if(server[1]){
                        pending.packet << i << player[i].x << player[i].y << player[i].heading << player[i].lineWidth << (player[i].invisible||player[i].gap[0]<0.0);
                    }
                }
            }
            if(server[1]){
                queuePacket(pending);
            }
            // End Update
            // Hit detection
            hitDetector(config,player);
        }
        else if(client[1]&&!roundFinished){
            // check key states and send if changed
            for(unsigned int i=0;i<player.size();i++){
                if(player[i].local){
                    // KeyL changed
                    if( (!player[i].left&&sf::Keyboard::isKeyPressed(player[i].keyL)) || (player[i].left&&!sf::Keyboard::isKeyPressed(player[i].keyL)) ){
                        player[i].left=sf::Keyboard::isKeyPressed(player[i].keyL);
                        // Send package
                        Pending pending;
                        pending.packet << Packet::KeyL << i << player[i].left;
                        queuePacket(pending);
                    }
                    // KeyR changed
                    if( (!player[i].right&&sf::Keyboard::isKeyPressed(player[i].keyR)) || (player[i].right&&!sf::Keyboard::isKeyPressed(player[i].keyR)) ){
                        player[i].right=sf::Keyboard::isKeyPressed(player[i].keyR);
                        // Send package
                        Pending pending;
                        pending.packet << Packet::KeyR << i << player[i].right;
                        queuePacket(pending);
                    }
                }
            }
            // Powerup
            if(!paused){
                elapsed=gameClock.restart().asSeconds();
                powerUpManager(config);
            };
        }
        // Pacer
        gamePacer.pace();
    }
    // shutdown
    updateThread[0]=false;
    updateThread[1]=false;
    updateThread[2]=false;
    std::cout << "Game update thread Ended" << std::endl;
}
//
void Game::newRound(const Config &config,std::vector<Player> &player){
    playerPowerupEffect.clear();
    // Random position for the players and heading
    for(unsigned int i=0;i<player.size();i++){
        player[i].newRound(config,*this);
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
        queuePacket(pending);
    }
    // Reset Some vars
    deathCount=0;
    roundFinished=false;
    gameFinished=false;
    endMessageSet=false;
    paused=true;
    round++;
    powerupField.clear();
    powerupEffect.clear();
    wallsAway=false;
    wallsAwayTimer=0;
    darkness=false;
    darknessTimer=0;
    morepowerups=0;
    powerupSpawnTime=config.powerupSpawnDelay;
    // ready 'm up!
    if(server[1]){
        // start countdownClock
        countdownClock.restart();
        countdownInt=3;
        Pending pending;
        pending.packet << Packet::Countdown;
        queuePacket(pending);
    }
    else if(!client[1]&&countdownEnabled){
        countdownClock.restart();
        countdownInt=3;
    }
}
//
void Game::hitDetector(const Config &config,std::vector<Player> &player){
    // Initializer vector to store death people
    std::vector<unsigned int> death_vec;
    // Do the magic
    for(unsigned int i=0;i<player.size();i++){
        if(player[i].death){continue;}
        // Player Position
        float x=player[i].x;
        float y=player[i].y;
        // Out of bounds check, Only if there are walls
        if(!wallsAway &&
           ( x-player[i].lineWidth/2<config.wallWidth ||
            x+player[i].lineWidth/2>config.windowWidth-config.statusWidth-config.wallWidth ||
            y-player[i].lineWidth/2<config.wallWidth ||
            y+player[i].lineWidth/2>config.windowHeight-config.wallWidth) ){
            death_vec.emplace_back(i);
        }
        else{
            // Line hit check
            // Only when not gapping or invisible
            if(player[i].gap[0]>0.0&&!player[i].invisible){
                bool hit=false;
                //
                for(unsigned int j=0;j<player.size()&&!hit;j++){
                    // Other Player
                    if(j!=i){
                        // Normal Detection
                        for(unsigned int k=0;k<player[j].line.getVertexCount();k++){
                            if( pow(x-player[j].line[k].position.x,2) + pow(y-player[j].line[k].position.y,2) < pow(player[i].lineWidth/2,2) ){
                                death_vec.emplace_back(i);
                                hit=true;
                                break;
                            }
                        }
                        // Head on detection
                        // Only when other player also isn't invisible
                        if(!hit&&player[j].gap[0]>0.0&&!player[j].invisible){
                            if( pow(x-player[j].x,2) + pow(y-player[j].y,2) < pow(player[i].lineWidth/2.0+player[j].lineWidth/2.0,2) ){
                                death_vec.emplace_back(i);
                                hit=true;
                                break;
                            }
                        }
                    }
                    // Own line
                    else if(j==i){
                        // Delay should be depending on speed and line width
                        unsigned int delay=1.5*config.shift;
                        // Linewidth scaling
                        if(player[j].lineWidth>config.lineWidth){
                            delay*=player[j].lineWidth/config.lineWidth;
                        }
                        // Right Angles; Bug: With right angles it is possible to move back over own line and not getting detected for a while
                        if(player[j].rightAngle){
                            delay*=1.5;
                        }
                        // Speed scaling
                        if(player[j].shift<config.shift){
                            delay*=config.shift/player[j].shift;
                        }
                        // Only when the line is long enough it will be checked
                        if(player[j].line.getVertexCount()>delay){
                            for(unsigned int k=0;k<player[j].line.getVertexCount()-delay;k++){
                                if( pow(x-player[j].line[k].position.x,2) + pow(y-player[j].line[k].position.y,2) < pow(player[i].lineWidth/2,2) ){
                                    death_vec.emplace_back(i);
                                    hit=true;
                                    break;
                                }
                            }
                        }
                    } // End own line hit check
                } // End for loop
            } // End frame>gap
        }// End else
    } // End for loop
    // Process the death
    if(death_vec.size()>0){
        //
        playerDeath(player,death_vec);
        // Stop round if less then two players
        if(deathCount>=player.size()-1){
            // End Round function
            endRound(config,player);
        }
    }
}
//
void Game::playerDeath(std::vector<Player> &player,const std::vector<unsigned int> &death_vec){
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
        queuePacket(pending);
    }
    // Declare people death. This is done after the points are given so this is done correctly
    for(unsigned int i=0;i<death_vec.size();i++){
        deathCount++;
        player[death_vec[i]].death=true;
    }
}
//
void Game::endRound(const Config &config,std::vector<Player> &player){
    // Show everything again
    darkness=false;
    // Round is always finished so set the var
    roundFinished=true;
    // Check if someone won the game
    int points=0; // Numer of points winner has
    roundWinner=-1; // The player number
    for(unsigned int i=0;i<player.size();i++){
        // check if player has the needed points to win
        if(player[i].points>=maxPoints){
            // check if player has more points then someone other that has more then maxPoints
            if(player[i].points>points){
                points=player[i].points;
                roundWinner=i;
            }
            // If two people have the same amount of points revert winner to -1 to let see that there is no winner yet
            else if(player[i].points==points){
                roundWinner=-1;
            }
        }
    }
    // Game finshes
    if(roundWinner>-1){
        gameFinished=true;
        if(updateThread[1]){
            updateThread[2]=true;
        }
        // Server Part :D
        if(server[1]){
            Pending pending;
            pending.packet << Packet::GameEnd << roundWinner;
            queuePacket(pending);
        }
    }
    // Game continues, someone has just won a round;
    else{
        for(unsigned int i=0;i<player.size();i++){
            if(!player[i].death){
                roundWinner=i;
                break;
            }
        }
        // Server Part :D
        if(server[1]){
            Pending pending;
            pending.packet << Packet::RoundEnd << roundWinner;
            queuePacket(pending);
        }
    }
}
//
void Game::quit(const Config &config){
    shutdown();
    // If client stop client thread and return to main menu
    if(client[1]){
        client[2]=true;
        switchMode(Game::Mode::mainMenu);
    }
    else{
        // If Server let clients also return to setup
        if(server[1]){
            Pending pending;
            pending.packet << Packet::ReturnSetup;
            queuePacket(pending);
        }
        //
        switchMode(Game::Mode::setup);
    }
}
//
void Game::shutdown(){
    if(updateThread[1]){
        updateThread[2]=true;
    }
    if(gameThread.joinable()){
        gameThread.join();
    }
}
//
void Game::powerUpManager(const Config &config,std::vector<Player> &player){
    // Deletes Powerups that are on the end of their lives
    for(unsigned int i=0;i<powerupField.size();i++){
        powerupField[i].time1+=elapsed;
        powerupField[i].time2-=elapsed;
        if(powerupField[i].time2<0.0){
            //
            if(server[1]){
                Pending pending;
                pending.packet << Packet::PowerupDeleteField << powerupField[i].id;
                queuePacket(pending);
            }
            //
            powerupField.erase(powerupField.begin()+i);
        }
    }
    // Remove Powerups for players
    bool deleted=false;
    for(unsigned int i=0;i<playerPowerupEffect.size();i++){
        playerPowerupEffect[i].time-=elapsed;
        if(playerPowerupEffect[i].time<0.0){
            // Only Right angle and inverted need to be reported
            if(server[1] &&
               (playerPowerupEffect[i].type==Powerup::Type::RightAngle || playerPowerupEffect[i].type==Powerup::Type::InvertKeys) ){
                Pending pending;
                pending.packet << Packet::PowerupDeletePlayer << playerPowerupEffect[i].id;
                queuePacket(pending);
            }
            //
            playerPowerupEffect.erase(playerPowerupEffect.begin()+i);
            deleted=true;
        }
    }
    // If deleted the effects have to be recalculated
    if(deleted){
        for(unsigned int i=0;i<player.size();i++){
                player[i].calculatePowerupEffect(config,*this);
        }
    }
    // Remove powerup effects that effect whole game
    for(unsigned j=0;j<powerupEffect.size();j++){
        powerupEffect[j].time-=elapsed;
        if(powerupEffect[j].time<0.0){
            if(powerupEffect[j].type==Powerup::Type::MorePowerups){
                morepowerups=3;
            }
            powerupEffect.erase(powerupEffect.begin()+j);
        }
    }
    // Process wallsAway and darkness
    if(wallsAway){
        wallsAwayTimer-=elapsed;
        if(wallsAwayTimer<0.0){
            wallsAway=false;
            wallsAwayTimer=0.0;
            if(server[1]){
                Pending pending;
                pending.packet << Packet::PowerupEnd << Powerup::Type::WallsAway;
                queuePacket(pending);
            }
        }
    }
    if(darkness){
        darknessTimer-=elapsed;
        if(darknessTimer<0.0){
            darkness=false;
            darknessTimer=0.0;
            if(server[1]){
                Pending pending;
                pending.packet << Packet::PowerupEnd << Powerup::Type::Darkness;
                queuePacket(pending);
            }
        }
    }
    // Spawn new ones
    // This is done every 'config.powerupSpawnCheck' seconds if it succeeds the second if
    powerupSpawnTime-=elapsed;
    if(powerupSpawnTime<0.0){
        powerupSpawnTime=config.powerupSpawnCheck;
        if(morepowerups>0||randSpawn(randGenerator)<config.powerupSpawnChance){
            //
            morepowerups--;
            //
            bool spawn=false;
            int iter=0; // Debug Variable
            // Try to make sure powerups doesn't spawn in front or on player
            int X,Y;
            while(!spawn&&iter<100){
                X=config.wallWidth + config.powerupRadius + rand() % (config.windowWidth-config.statusWidth-2*config.wallWidth-2*config.powerupRadius+1); // Rand x position within walls
                Y=config.wallWidth + config.powerupRadius + rand() % (config.windowHeight-2*config.wallWidth-2*config.powerupRadius+1);
                spawn=true;
                // Player hit check
                for(unsigned int i=0;i<player.size();i++){
                    if(player[i].death){continue;}
                    // check 1, Not on player
                    if( pow(player[i].x-X,2) + pow(player[i].y-Y,2) < pow(config.powerupRadius,2) ) {
                        spawn=false;
                        break;
                    }
                    // check 2, not directly in front of player
                    if( pow(X - (player[i].x+cos(player[i].heading*config.powerupSafeDistance)), 2) + pow(Y - (player[i].y+sin(player[i].heading*config.powerupSafeDistance)), 2) < pow(config.powerupSafeRadius, 2) ){
                        spawn=false;
                        break;
                    }
                }
                iter++;
                // Debug
                if(iter%25==0){
                    std::cout << iter <<std::endl;
                }
            }
            // Choose a powerup
            Powerup::Type type;
            Powerup::Impact impact;
            int place;
            choosePowerUp(type,impact,place);
            // Calculate disappear time
            float D=config.powerupMinDisappear + ( ( rand() % (config.powerupRandDisappear+1) ) / 1000.0 ); // Disappear Time
            // Set ID so it easier to identify
            int id=std::time(nullptr);
            // Store powerup
            powerupField.emplace_back(X,Y,type,impact,D,id,place);
            //
            if(server[1]){
                Pending pending;
                pending.packet << Packet::PowerupSpawn << id << X << Y << type << impact << D;
                queuePacket(pending);
            }
        }
    }
    // End with checking if a player hits a powerup
    // i=player
    // j=powerup
    // k=player within effect for enemy's or all
    for(unsigned int i=0;i<player.size();i++){
        if(player[i].death){continue;}
        for(unsigned int j=0;j<powerupField.size();j++){
            float dx=player[i].x-powerupField[j].x;
            float dy=player[i].y-powerupField[j].y;
            float radius = config.powerupRadius + player[i].lineWidth/2.0;
            if( (dx*dx) + (dy*dy) < radius*radius ) {
                // Effect Time
                int D=powerups[powerupField[j].place].effectMinDisappear + ( ( rand() % (powerups[powerupField[j].place].effectRandDisappear+1) ) / 1000.0 );
                while(powerupField[j].type!=Powerup::Type::Multiplier && player[i].multiplier>0){
                    D*=config.lengthMultiplier;
                    player[i].multiplier--;
                }
                //
                int id=powerupField[j].id;
                // Question Mark
                while(powerupField[j].type==Powerup::Type::QuestionMark){
                    choosePowerUp(powerupField[j].type,powerupField[j].impact,powerupField[j].place);
                }
                // Effecter
                // Player Effects
                switch (powerupField[j].type){
                    case Powerup::Type::Slow:
                    case Powerup::Type::Fast:
                    case Powerup::Type::Small:
                    case Powerup::Type::Big:
                    case Powerup::Type::RightAngle:
                    case Powerup::Type::Invisible:
                    case Powerup::Type::InvertKeys:
                    case Powerup::Type::Sine:
                    case Powerup::Type::Gap:
                    case Powerup::Type::Glitch:
                    case Powerup::Type::Radius:
                        playerPowerupEffect.emplace_back(i,powerupField[j].type,powerupField[j].impact,D,id);
                        for(unsigned int k=0;k<player.size();k++){
                            player[k].calculatePowerupEffect(config,*this);
                        }
                        break;
                    case Powerup::Type::NoTurtle:
                        for(unsigned int k=0;k<player.size();k++){
                            player[k].calculateNoTurtleEffect(config,i,powerupField[j].impact);
                        }
                        break;
                    case Powerup::Type::Clear:
                        for(unsigned int k=0;k<player.size();k++){
                            player[k].line.clear();
                            player[k].noTurtleLine.clear();
                        }
                        break;
                    case Powerup::Type::WallsAway:
                        wallsAwayTimer+=D;
                        wallsAway=true;
                        break;
                    case Powerup::Type::MorePowerups:
                        powerupEffect.emplace_back(Powerup::Type::MorePowerups,D,id);
                        break;
                    case Powerup::Type::Darkness:
                        darknessTimer+=D;
                        darkness=true;
                        break;
                    case Powerup::Type::Bomb:
                        powerUpBomb(config,player,i,j);
                        break;
                    case Powerup::Type::Multiplier:
                        player[i].multiplier++;
                        break;
                    default:
                        std::cout << "Powerup case uncaptured!" << std::endl;
                        break;
                }
                //
                if(server[1]){
                    Pending pending;
                    pending.packet << Packet::PowerupHit << i << id;
                    queuePacket(pending);
                }
                // Remove powerup!
                powerupField.erase(powerupField.begin()+j);
            } // End within radius check
        }// End powerup loop
    }// End player Hit check
    //
}
//
void Game::powerUpManager(const Config &config){
    for(unsigned int i=0;i<powerupField.size();i++){
        powerupField[i].time1+=elapsed;
        powerupField[i].time2-=elapsed;
    }
}
//
void Game::choosePowerUp(Powerup::Type &type, Powerup::Impact &impact, int &place){
    int random=randPowerup(randGenerator);
    int low=0;
    for(unsigned int i=0;i<powerups.size();i++){
        if( random>=low && random<(low+powerups[i].spawnChance) ){
            type=powerups[i].type;
            impact=powerups[i].impact;
            place=i;
            return;
        }
        else{
            low+=powerups[i].spawnChance;
        }
    }
}
//
void Game::powerUpBomb(const Config &config,std::vector<Player> &player, const int &i, const unsigned int &bombNumber){
    // Remove lines
    int xc,yc;
    for(unsigned int k=0;k<player.size();k++){
        for(unsigned int l=0;l+3<player[k].line.getVertexCount();l=l+4){
            // Find out if within blast radius
            xc=(player[k].line[l].position.x+player[k].line[l+1].position.x+player[k].line[l+2].position.x+player[k].line[l+3].position.x)/4;
            yc=(player[k].line[l].position.y+player[k].line[l+1].position.y+player[k].line[l+2].position.y+player[k].line[l+3].position.y)/4;
            if( (xc-player[i].x)*(xc-player[i].x) + (yc-player[i].y)*(yc-player[i].y) < config.bombRadius*config.bombRadius ){
                // Erase doesn't work due to the wrapper. I just move the lines out of sight
                player[k].line[l].position=sf::Vector2f(-100, -100);
                player[k].line[l+1].position=sf::Vector2f(-100, -100);
                player[k].line[l+2].position=sf::Vector2f(-100, -100);
                player[k].line[l+3].position=sf::Vector2f(-100, -100);
            }
        }
    }
    // Remove Powerups
    for(unsigned int k=0;k<powerupField.size();k++){
        if(k!=bombNumber){
            xc=player[i].x-powerupField[k].x;
            yc=player[i].y-powerupField[k].y;
            if( xc*xc + yc*yc < config.bombRadius*config.bombRadius){
                powerupField.erase(powerupField.begin()+k);
                k--;
            }
        }
    }
}
//
void Game::pause(const bool &pause){
    if(pause){
        paused=true;
        if(server[1]){
            Pending pending;
            pending.packet << Packet::Pause << true;
            queuePacket(pending);
        }
    }
    else{
        paused=false;
        gameClock.restart();
        packetClock.restart();
        if(server[1]){
            Pending pending;
            pending.packet << Packet::Pause << false;
            queuePacket(pending);
        }
        countdownInt=0;
    }
}
//
void Game::optionsChanged(RendererObjects &objects){
    if(server[1]){
        Pending pending;
        pending.packet << Packet::Options << maxPoints << powerupEnabled << multiplePlayersEnabled;
        queuePacket(pending);
    }
    objects.setOptions(*this);
}
//
void Game::queuePacket(Pending &packet){
    if(packet.sendID.size()==0){
        packet.sendID.emplace_back(-1);
    }
    packetMutex.lock();
    packets.push_back(packet);
    packetMutex.unlock();
}
//
