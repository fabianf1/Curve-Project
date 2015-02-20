// Contains functions and constructors for the Renderer class
// Needed Header
#include "renderer.h"
//
void Renderer::start(const Config &config,Game &game,const std::vector<Player> &player){
    // Window
    window.create(sf::VideoMode(config.windowWidth, config.windowHeight), config.title, config.windowStyle, sf::ContextSettings(24,8,config.windowAntialiasing));
    window.setIcon(202,202,config.icon.getPixelsPtr());
    window.clear(config.windowBackgroundColor);
    window.display();
    // Limit fps
    window.setFramerateLimit(config.fps);
    // start Render thread
    window.setActive(false);
    renderThread = std::thread(&Renderer::thread,this,std::cref(config),std::ref(game),std::cref(player));
    // Done
}
// Render thread Function
void Renderer::thread(const Config &config,Game &game,const std::vector<Player> &player){
    std::cout << "Render thread Started" << std::endl;
    // start main loop
    while(window.isOpen()){
        window.clear(config.windowBackgroundColor);
        // Lock the mode mutex. Not rendering while mode is changing solves font problems
        game.modeMutex.lock();
        // Render the things
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
        // Unlock
        game.modeMutex.unlock();
        //
        window.display();
        game.frame++;
        // FPS
        if(game.frame%(config.fps)==0){
            sf::Time temp=game.fpsClock.restart();
            //std::cout << temp.asSeconds() << std::endl;
            objects.g_fps[1].setString(int2str( (1.0/temp.asSeconds())*(config.fps) ));
        }
    }
    std::cout << "Render thread Stopped" << std::endl;
}
//
void Renderer::mainMenu(){
    // Title
    window.draw(objects.m_title);
    // Buttons
    // Create Button
    window.draw(objects.m_create);
    // Join
    window.draw(objects.m_join);
    // Quit
    window.draw(objects.m_quit);
    // Prompt
    objects.m_ipPrompt.draw(window);
}
//
void Renderer::setup(const Config &config,const Game &game,const std::vector<Player> &player){
    //
    window.draw(objects.s_title);
    window.draw(objects.s_name);
    window.draw(objects.s_left);
    window.draw(objects.s_right);
    // Vectors
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
    // More
    if(objects.vectorLength<config.maxPlayers&&( (game.client[1]&&!player[game.id].ready&&game.multiplePlayersEnabled) || !game.client[1] ) ){
        window.draw(objects.s_add);
    }
    // Options
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
    // draw Player Things
    for(unsigned int i=0;i<player.size();i++){
        //player[i].draw(window);
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
            // Line
            int xc;
            int yc;
            // create a quad; Maybe change it to pointers;
            sf::VertexArray quad(sf::Quads, 4);
            quad[0].color=player[i].color;
            quad[1].color=player[i].color;
            quad[2].color=player[i].color;
            quad[3].color=player[i].color;
            //
            for(unsigned int j=0;j+3<player[i].line.getVertexCount();j=j+4){
                xc=(player[i].line[j].position.x+player[i].line[j+1].position.x+player[i].line[j+2].position.x+player[i].line[j+3].position.x)/4;
                yc=(player[i].line[j].position.y+player[i].line[j+1].position.y+player[i].line[j+2].position.y+player[i].line[j+3].position.y)/4;
                for(unsigned int k=0;k<player.size();k++){
                    // Calculate center
                    if( (xc-player[k].x)*(xc-player[k].x) + (yc-player[k].y)*(yc-player[k].y) < (config.darknessRadius*config.darknessRadius) ){
                        quad[0].position = sf::Vector2f(player[i].line[j].position.x, player[i].line[j].position.y);
                        quad[1].position = sf::Vector2f(player[i].line[j+1].position.x, player[i].line[j+1].position.y);
                        quad[2].position = sf::Vector2f(player[i].line[j+2].position.x, player[i].line[j+2].position.y);
                        quad[3].position = sf::Vector2f(player[i].line[j+3].position.x, player[i].line[j+3].position.y);
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
    //window.draw(objects.g_packet[0]);
    //window.draw(objects.g_packet[1]);
    window.draw(objects.g_quit);
    if(game.roundFinished&&game.endMessageSet){window.draw(objects.g_endRoundMessage);}
    if(game.countdownInt>0){window.draw(objects.g_countdown);}
}
//
void Renderer::powerUp(const Config &config,const Game &game,const std::vector<Player> &player){
    for(unsigned int i=0;i<game.powerupField.size();i++){
        // check if withing FOV
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
        // Set opacity
        int opacity=255;
        if(game.powerupField[i].time1<config.powerupFadeTime){
            opacity=(game.powerupField[i].time1/config.powerupFadeTime)*255;
        }
        else if(game.powerupField[i].time2<config.powerupFadeTime){
            opacity=(game.powerupField[i].time2/config.powerupFadeTime)*255;
        }
        sf::Color color(255,255,255,opacity);
        // Choose Sprite
        sf::Sprite *draw;
        // Fast
        if(game.powerupField[i].type==Powerup::Type::Fast){
            if(game.powerupField[i].impact==Powerup::Impact::Self){
                draw=&sprite.fastGreen;
            }
            else if(game.powerupField[i].impact==Powerup::Impact::Other){
                draw=&sprite.fastRed;
            }
        }
        // Slow
        else if(game.powerupField[i].type==Powerup::Type::Slow){
            if(game.powerupField[i].impact==Powerup::Impact::Self){
                draw=&sprite.slowGreen;
            }
            else if(game.powerupField[i].impact==Powerup::Impact::Other){
                draw=&sprite.slowRed;
            }
        }
        // Small
        else if(game.powerupField[i].type==Powerup::Type::Small){
            if(game.powerupField[i].impact==Powerup::Impact::Self){
                draw=&sprite.smallGreen;
            }
            else if(game.powerupField[i].impact==Powerup::Impact::Other){
                draw=&sprite.smallRed;
            }
        }
        // Big
        else if(game.powerupField[i].type==Powerup::Type::Big){
            if(game.powerupField[i].impact==Powerup::Impact::Self){
                draw=&sprite.bigGreen;
            }
            else if(game.powerupField[i].impact==Powerup::Impact::Other){
                draw=&sprite.bigRed;
            }
        }
        // Right Angle
        else if(game.powerupField[i].type==Powerup::Type::RightAngle){
            if(game.powerupField[i].impact==Powerup::Impact::Self){
                draw=&sprite.angleGreen;
            }
            else if(game.powerupField[i].impact==Powerup::Impact::Other){
                draw=&sprite.angleRed;
            }
        }
        // Clear
        else if(game.powerupField[i].type==Powerup::Type::Clear){
            draw=&sprite.clearBlue;
        }
        // Invisible
        else if(game.powerupField[i].type==Powerup::Type::Invisible){
            draw=&sprite.invisibleGreen;
        }
        // Walls Away
        else if(game.powerupField[i].type==Powerup::Type::WallsAway){
            draw=&sprite.wallsAwayBlue;
        }
        // More
        else if(game.powerupField[i].type==Powerup::Type::MorePowerups){
            draw=&sprite.morePowerupsBlue;
        }
        // Inverted
        else if(game.powerupField[i].type==Powerup::Type::InvertKeys){
            draw=&sprite.invertedRed;
        }
        // Question Mark
        else if(game.powerupField[i].type==Powerup::Type::QuestionMark){
            draw=&sprite.questionMarkBlue;
        }
        // Darkness
        else if(game.powerupField[i].type==Powerup::Type::Darkness){
            draw=&sprite.darknessBlue;
        }
        // Gap
        else if(game.powerupField[i].type==Powerup::Type::Gap){
            draw=&sprite.gapRed;
        }
        // Bomb
        else if(game.powerupField[i].type==Powerup::Type::Bomb){
            draw=&sprite.bombBlue;
        }
        // Sine
        else if(game.powerupField[i].type==Powerup::Type::Sine){
            draw=&sprite.sineRed;
        }
        // Glitch
        else if(game.powerupField[i].type==Powerup::Type::Glitch){
            draw=&sprite.glitchBlue;
        }
        // Radius
        else if(game.powerupField[i].type==Powerup::Type::Radius){
            draw=&sprite.radiusBlue;
        }
        else{
            std::cout << "Error! No sprite!" << int2str( game.powerupField[i].place ) << std::endl;
            draw=&sprite.questionMarkBlue;
        }
        // Do the things
        draw->setPosition( game.powerupField[i].x-config.powerupRadius , game.powerupField[i].y-config.powerupRadius );
        draw->setColor(color);
        window.draw(*draw);
    }
    //
}
//
void Renderer::shutdown(){
    window.close();
    if(renderThread.joinable()){
        renderThread.join();
    }
}
//
