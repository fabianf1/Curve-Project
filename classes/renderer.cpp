// Contains functions and constructors for the Renderer class
// Needed Header
#include "renderer.h"
//
void Renderer::start(const Config &config,Game &game,const std::vector<Player> &player){
    // Create and initialize render window
    std::cout << "Initializing window" << std::endl;
    window.create(sf::VideoMode(config.windowWidth, config.windowHeight), config.title, config.windowStyle, sf::ContextSettings(24,8,config.windowAntialiasing));
    window.setIcon(202,202,config.icon.getPixelsPtr());
    window.clear(config.windowBackgroundColor);
    window.setFramerateLimit(config.fps);
    window.display();
    // Start the render thread
    std::cout << "Starting render thread" << std::endl;
    window.setActive(false);
    renderThread = std::thread(&Renderer::thread,this,std::cref(config),std::ref(game),std::cref(player));
}
// Render thread Function
void Renderer::thread(const Config &config,Game &game,const std::vector<Player> &player){
    std::cout << "Render thread Started" << std::endl;
    // start main loop
    while(window.isOpen()){
        window.clear(config.windowBackgroundColor);
        // Locking the mutex when changing modes solves font rendering problems
        game.modeMutex.lock();
        if(game.mode==Game::Mode::mainMenu){
            mainMenu();
        }
        else if(game.mode==Game::Mode::setup){
            setup(config,game,player);
        }
        else if(game.mode==Game::Mode::Play){
            play(config,game,player);
            if(game.powerupEnabled){powerUp(config,game,player);}
        }
        game.modeMutex.unlock();
        // Draw everything to window and update fps
        window.display();
        game.frame++;
        if(game.frame%(config.fps)==0){
            sf::Time temp=game.fpsClock.restart();
            objects.g_fps[1].setString(int2str( (1.0/temp.asSeconds())*(config.fps) ));
        }
    }
    std::cout << "Render thread Stopped" << std::endl;
}
//
void Renderer::mainMenu(){
    window.draw(objects.m_title);
    window.draw(objects.m_version);
    window.draw(objects.m_create);
    window.draw(objects.m_join);
    window.draw(objects.m_quit);
    objects.m_ipPrompt.draw(window);
}
//
void Renderer::setup(const Config &config,const Game &game,const std::vector<Player> &player){
    window.draw(objects.s_title);
    window.draw(objects.s_name);
    window.draw(objects.s_left);
    window.draw(objects.s_right);
    // Render players
    for(unsigned int i=0;i<objects.vectorLength;i++){
        objects.s_names[i].draw(window);
        if(player[i].local){
            window.draw(objects.s_leftButton[i]);
            window.draw(objects.s_rightButton[i]);
        }
        else{
            window.draw(objects.s_status[i]);
        }
        // Local: Can kick all. Server: Can kick all but first player. Client: Can kick local players except first
        if( (game.client[1] && i!=game.id && player[i].local) || (game.server[1] && i!=0 ) || (!game.server[1]&&!game.client[1]) ){
            window.draw(objects.s_kick[i]);
        }
    }
    if(objects.vectorLength<config.maxPlayers&&( (game.client[1]&&!player[game.id].ready&&game.multiplePlayersEnabled) || !game.client[1] ) ){
        window.draw(objects.s_add);
    }
    // Render ptions
    window.draw(objects.s_options);
    window.draw(objects.s_maxPoints);
    window.draw(objects.s_max10);
    window.draw(objects.s_max20);
    window.draw(objects.s_max40);
    window.draw(objects.s_powerups);
    window.draw(objects.s_powerupOn);
    window.draw(objects.s_powerupOff);
    if(!game.client[1]&&!game.server[1]){
        window.draw(objects.s_countdown);
        window.draw(objects.s_countdownOn);
        window.draw(objects.s_countdownOff);
    }
    else{
        window.draw(objects.s_multiplePlayers);
        window.draw(objects.s_multiplePlayersOn);
        window.draw(objects.s_multiplePlayersOff);
    }
    // Buttons
    window.draw(objects.s_server);
    window.draw(objects.s_start);
    window.draw(objects.s_quit);
}
//
void Renderer::play(const Config &config,const Game &game,const std::vector<Player> &player){
    // Draw player lines, circles and rectangles
    for(unsigned int i=0;i<player.size();i++){
        if(!game.darkness){
            window.draw(player[i].line);
            if(!player[i].rightAngle){
                window.draw(player[i].circle);
            }
            else{
                window.draw(player[i].rectangle);
            }
        }
        else{
            int xc; // To define x-center of line
            int yc; // To define y-center of line
            // Create quad; This has to be done to be able to draw parts of the line; Or at least I think so
            sf::VertexArray quad(sf::Quads, 4);
            //
            for(unsigned int j=0;j+3<player[i].line.getVertexCount();j=j+4){
                xc=(player[i].line[j].position.x+player[i].line[j+1].position.x+player[i].line[j+2].position.x+player[i].line[j+3].position.x)/4;
                yc=(player[i].line[j].position.y+player[i].line[j+1].position.y+player[i].line[j+2].position.y+player[i].line[j+3].position.y)/4;
                for(unsigned int k=0;k<player.size();k++){
                    // Calculate center
                    if( (xc-player[k].x)*(xc-player[k].x) + (yc-player[k].y)*(yc-player[k].y) < (config.darknessRadius*config.darknessRadius) ){
                        quad[0] = player[i].line[j];
                        quad[1] = player[i].line[j+1];
                        quad[2] = player[i].line[j+2];
                        quad[3] = player[i].line[j+3];
                        window.draw(quad);
                        break;
                    }
                }
            }
            // Circle
            for(unsigned int k=0;k<player.size();k++){
                if(k==i || ( (player[i].x-player[k].x)*(player[i].x-player[k].x) + (player[i].y-player[k].y)*(player[i].y-player[k].y) < (config.darknessRadius*config.darknessRadius) ) ){
                    if(!player[i].rightAngle){
                        window.draw(player[i].circle);
                    }
                    else{
                        window.draw(player[i].rectangle);
                    }
                    break;
                }
            }
        }
    }
    // check if walls away effect is there and change wallColor(Decouple from fps?)
    sf::Color color=config.wallColor;
    if(game.wallsAway){
        int remainder=(game.frame% (config.fps*2) )-config.fps;
        if(remainder<0){
            color.a=255/(config.fps)*(-1)*remainder;
        }
        else{
            color.a=255/(config.fps)*remainder;
        }
    }
    // draw Walls
    window.draw(objects.g_leftWall);
    objects.g_leftWall.setFillColor(color);
    window.draw(objects.g_topWall);
    objects.g_topWall.setFillColor(color);
    window.draw(objects.g_rightWall);
    objects.g_rightWall.setFillColor(color);
    window.draw(objects.g_bottomWall);
    objects.g_bottomWall.setFillColor(color);
    // draw Status Menu
    window.draw(objects.g_scoreTitle);
    for(unsigned int i=0;i<objects.vectorLength;i++){
        window.draw(objects.g_names[i]);
        window.draw(objects.g_scores[i]);
    }
    window.draw(objects.g_round[0]);
    window.draw(objects.g_round[1]);
    window.draw(objects.g_fps[0]);
    window.draw(objects.g_fps[1]);
    if(game.client[1]){
        window.draw(objects.g_packet[0]);
        objects.g_packet[1].setString(int2str(config.gameUpdateThreadMinRate/game.packetTime));
        window.draw(objects.g_packet[1]);
    }
    window.draw(objects.g_quit);
    if(game.roundFinished&&game.endMessageSet){window.draw(objects.g_endRoundMessage);}
    if(game.countdownInt>0){window.draw(objects.g_countdown);}
}
//
void Renderer::powerUp(const Config &config,const Game &game,const std::vector<Player> &player){
    for(unsigned int i=0;i<game.powerupField.size();i++){
        // If darkness powerup is enabled only draw powerups within FoV
        if(game.darkness){
            bool draw=false;
            int diffx;
            int diffy;
            for(unsigned int j=0;j<player.size();j++){
                diffx=(player[j].x-game.powerupField[i].x);
                diffy=(player[j].y-game.powerupField[i].y);
                if( diffx*diffx+diffy*diffy < (config.darknessRadius*config.darknessRadius)/4 ){
                    draw=true;
                    break;
                }
            }
            if(!draw){
                continue;
            }
        }
        // Set opacity for fade in and out effects
        int opacity=255;
        if(game.powerupField[i].time1<config.powerupFadeTime){
            opacity=(game.powerupField[i].time1/config.powerupFadeTime)*255;
        }
        else if(game.powerupField[i].time2<config.powerupFadeTime){
            opacity=(game.powerupField[i].time2/config.powerupFadeTime)*255;
        }
        sf::Color color(255,255,255,opacity);
        // Set sprite to draw
        sf::Sprite *draw;
        if(game.powerupField[i].type==Powerup::Type::Fast){
            if(game.powerupField[i].impact==Powerup::Impact::Self){
                draw=&sprite.fastGreen;
            }
            else if(game.powerupField[i].impact==Powerup::Impact::Other){
                draw=&sprite.fastRed;
            }
        }
        else if(game.powerupField[i].type==Powerup::Type::Slow){
            if(game.powerupField[i].impact==Powerup::Impact::Self){
                draw=&sprite.slowGreen;
            }
            else if(game.powerupField[i].impact==Powerup::Impact::Other){
                draw=&sprite.slowRed;
            }
        }
        else if(game.powerupField[i].type==Powerup::Type::Small){
            if(game.powerupField[i].impact==Powerup::Impact::Self){
                draw=&sprite.smallGreen;
            }
            else if(game.powerupField[i].impact==Powerup::Impact::Other){
                draw=&sprite.smallRed;
            }
        }
        else if(game.powerupField[i].type==Powerup::Type::Big){
            if(game.powerupField[i].impact==Powerup::Impact::Self){
                draw=&sprite.bigGreen;
            }
            else if(game.powerupField[i].impact==Powerup::Impact::Other){
                draw=&sprite.bigRed;
            }
        }
        else if(game.powerupField[i].type==Powerup::Type::RightAngle){
            if(game.powerupField[i].impact==Powerup::Impact::Self){
                draw=&sprite.angleGreen;
            }
            else if(game.powerupField[i].impact==Powerup::Impact::Other){
                draw=&sprite.angleRed;
            }
        }
        else if(game.powerupField[i].type==Powerup::Type::Clear){
            draw=&sprite.clearBlue;
        }
        else if(game.powerupField[i].type==Powerup::Type::Invisible){
            draw=&sprite.invisibleGreen;
        }
        else if(game.powerupField[i].type==Powerup::Type::WallsAway){
            draw=&sprite.wallsAwayBlue;
        }
        else if(game.powerupField[i].type==Powerup::Type::MorePowerups){
            draw=&sprite.morePowerupsBlue;
        }
        else if(game.powerupField[i].type==Powerup::Type::InvertKeys){
            draw=&sprite.invertedRed;
        }
        else if(game.powerupField[i].type==Powerup::Type::QuestionMark){
            draw=&sprite.questionMarkBlue;
        }
        else if(game.powerupField[i].type==Powerup::Type::Darkness){
            draw=&sprite.darknessBlue;
        }
        else if(game.powerupField[i].type==Powerup::Type::Gap){
            draw=&sprite.gapRed;
        }
        else if(game.powerupField[i].type==Powerup::Type::Bomb){
            draw=&sprite.bombBlue;
        }
        else if(game.powerupField[i].type==Powerup::Type::Sine){
            draw=&sprite.sineRed;
        }
        else if(game.powerupField[i].type==Powerup::Type::Glitch){
            if(game.powerupField[i].impact==Powerup::Impact::All){
                draw=&sprite.glitchBlue;
            }
            else if(game.powerupField[i].impact==Powerup::Impact::Other){
                draw=&sprite.glitchRed;
            }
        }
        else if(game.powerupField[i].type==Powerup::Type::Radius){
            draw=&sprite.radiusBlue;
        }
        else{
            std::cout << "Error! No sprite!" << int2str( game.powerupField[i].place ) << std::endl;
            draw=&sprite.questionMarkBlue;
        }
        // Set sprite and draw
        draw->setPosition( game.powerupField[i].x-config.powerupRadius , game.powerupField[i].y-config.powerupRadius );
        draw->setColor(color);
        window.draw(*draw);
    }
}
//
void Renderer::shutdown(){
    window.close();
    if(renderThread.joinable()){
        renderThread.join();
    }
}
//
