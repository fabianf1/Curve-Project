// Contains functions and constructors for the Renderer class
// Needed Header
#include "renderer.h"
//
void Renderer::Start(const Config &config,Game &game,std::vector<Player> &player){
    // Window
    window.create(sf::VideoMode(config.window_width, config.window_height), config.title, config.window_style, sf::ContextSettings(24,8,config.window_antialising));
    window.setIcon(202,202,config.icon.getPixelsPtr());
    window.clear(config.window_backgroundcolor);
    window.display();
    // Limit fps
    window.setFramerateLimit(config.fps);
    // Start Render thread
    window.setActive(false);
    thread = std::thread(&Renderer::Thread,this,std::cref(config),std::ref(game),std::ref(player));
    // Done
}
// Render Thread Function
void Renderer::Thread(const Config &config,Game &game,std::vector<Player> &player){
    std::cout << "Render Thread Started" << std::endl;
    sf::sleep(sf::milliseconds(1000));
    // Start main loop
    while(window.isOpen()){
        window.clear(config.window_backgroundcolor);
        //
        if(game.mode==Game::Mode::Main_Menu){
            Main_Menu(config,game,player);
        }
        else if(game.mode==Game::Mode::Setup){
            Setup(config,game,player);
        }
        else if(game.mode==Game::Mode::Play){
            Play(config,game,player);
            if(game.powerup_enabled){PowerUp(config,game);}
        }
        //
        window.display();
        game.frame++;
        // FPS
        if(game.frame%(config.fps)==0){
            objects.g_fps[1].setString(int2string( (1.0/game.fps_clock.restart().asSeconds())*(config.fps) ));
        }
    }
    std::cout << "Render Thread Stopped" << std::endl;
}
//
void Renderer::Main_Menu(const Config &config,Game &game,std::vector<Player> &player){
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
    objects.m_ip_prompt.Draw(window);
}
//
void Renderer::Setup(const Config &config,Game &game,std::vector<Player> &player){
    //
    window.draw(objects.s_title);
    window.draw(objects.s_name);
    window.draw(objects.s_left);
    window.draw(objects.s_right);
    // Vectors
    for(unsigned int i=0;i<objects.vector_length;i++){
        objects.s_names[i].Draw(window);
        if(player[i].local){
            window.draw(objects.s_lbutton[i]);
            window.draw(objects.s_rbutton[i]);
        }
        else{
            window.draw(objects.s_status[i]);
        }
        if(!game.client[1]&& (!player[i].server||!game.server[1]) ){window.draw(objects.s_kick[i]);}
    }
    // More
    if(objects.vector_length<6&&!game.client[1]&&!game.server[1]){
        window.draw(objects.s_add);
    }
    //window.draw(objects.s_name_pointer);
    // Options
    window.draw(objects.s_options);
    window.draw(objects.s_maxpoints);
    window.draw(objects.s_max10);
    window.draw(objects.s_max20);
    window.draw(objects.s_max40);
    window.draw(objects.s_powerups);
    window.draw(objects.s_powerupon);
    window.draw(objects.s_powerupoff);
    if(!game.client[1]&&!game.server[1]){
        window.draw(objects.s_countdown);
        window.draw(objects.s_countdownon);
        window.draw(objects.s_countdownoff);
    }
    // Buttons
    window.draw(objects.s_server);
    window.draw(objects.s_start);
    window.draw(objects.s_quit);
}
//
void Renderer::Play(const Config &config,Game &game,std::vector<Player> &player){
    // Draw Player Things
    for(unsigned int i=0;i<player.size();i++){
        player[i].Draw(window);
    }
    // Check if walls away effect is there and change wallcolor(Decouple from fps?)
    sf::Color color=config.wallcolor;
    if(game.wallsaway){
        int remainder=(game.frame% (config.fps*2) )-config.fps;
        if(remainder<0){
            color.a=255/(config.fps)*(-1)*remainder;
        }
        else{
            color.a=255/(config.fps)*remainder;
        }
    }
    // Draw Walls
    window.draw(objects.g_leftwall);
    objects.g_leftwall.setFillColor(color);
    window.draw(objects.g_topwall);
    objects.g_topwall.setFillColor(color);
    window.draw(objects.g_rightwall);
    objects.g_rightwall.setFillColor(color);
    window.draw(objects.g_bottomwall);
    objects.g_bottomwall.setFillColor(color);
    // Draw Status Menu
    window.draw(objects.g_scores);
    for(unsigned int i=0;i<objects.vector_length;i++){
        window.draw(objects.g_names[i]);
        window.draw(objects.g_score[i]);
    }
    window.draw(objects.g_round[0]);
    window.draw(objects.g_round[1]);
    window.draw(objects.g_frame[0]);
    window.draw(objects.g_frame[1]);
    window.draw(objects.g_fps[0]);
    window.draw(objects.g_fps[1]);
    //window.draw(objects.g_packet[0]);
    //window.draw(objects.g_packet[1]);
    window.draw(objects.g_quit);
    if(game.round_finished&&game.end_message_set){window.draw(objects.g_end_round_message);}
    if(game.countdown_int>0){window.draw(objects.g_countdown);}
}
//
void Renderer::PowerUp(const Config &config,Game &game){
    for(unsigned int i=0;i<game.powerup_field.size();i++){
        // find opacity
        int opacity=255;
        if(game.powerup_field[i].time1<config.powerup_fade_time){
            opacity=(game.powerup_field[i].time1/config.powerup_fade_time)*255;
        }
        else if(game.powerup_field[i].time2<config.powerup_fade_time){
            opacity=(game.powerup_field[i].time2/config.powerup_fade_time)*255;
        }
        sf::Color color(255,255,255,opacity);
        // Choose Sprite
        sf::Sprite *draw;
        // Fast
        if(game.powerup_field[i].type==Powerup::Type::Fast){
            if(game.powerup_field[i].impact==Powerup::Impact::Self){
                draw=&sprite.fastgreen;
            }
            else if(game.powerup_field[i].impact==Powerup::Impact::Other){
                draw=&sprite.fastred;
            }
        }
        // Slow
        else if(game.powerup_field[i].type==Powerup::Type::Slow){
            if(game.powerup_field[i].impact==Powerup::Impact::Self){
                draw=&sprite.slowgreen;
            }
            else if(game.powerup_field[i].impact==Powerup::Impact::Other){
                draw=&sprite.slowred;
            }
        }
        // Small
        else if(game.powerup_field[i].type==Powerup::Type::Small){
            if(game.powerup_field[i].impact==Powerup::Impact::Self){
                draw=&sprite.smallgreen;
            }
            else if(game.powerup_field[i].impact==Powerup::Impact::Other){
                draw=&sprite.smallred;
            }
        }
        // Big
        else if(game.powerup_field[i].type==Powerup::Type::Big){
            if(game.powerup_field[i].impact==Powerup::Impact::Self){
                draw=&sprite.biggreen;
            }
            else if(game.powerup_field[i].impact==Powerup::Impact::Other){
                draw=&sprite.bigred;
            }
        }
        // Right Angle
        else if(game.powerup_field[i].type==Powerup::Type::Right_Angle){
            if(game.powerup_field[i].impact==Powerup::Impact::Self){
                draw=&sprite.anglegreen;
            }
            else if(game.powerup_field[i].impact==Powerup::Impact::Other){
                draw=&sprite.anglered;
            }
        }
        // Clear
        else if(game.powerup_field[i].type==Powerup::Type::Clear){
            draw=&sprite.clearblue;
        }
        // Invisible
        else if(game.powerup_field[i].type==Powerup::Type::Invisible){
            draw=&sprite.invisiblegreen;
        }
        // Walls Away
        else if(game.powerup_field[i].type==Powerup::Type::Walls_Away){
            draw=&sprite.wallsawayblue;
        }
        // More
        else if(game.powerup_field[i].type==Powerup::Type::More_Powerups){
            draw=&sprite.morepowerblue;
        }
        // Inverted
        else if(game.powerup_field[i].type==Powerup::Type::Invert_Keys){
            draw=&sprite.invertedred;
        }
        // Question Mark
        else if(game.powerup_field[i].type==Powerup::Type::Question_Mark){
            draw=&sprite.questionblue;
        }
        // Do the things
        draw->setPosition( game.powerup_field[i].x-config.powerup_radius , game.powerup_field[i].y-config.powerup_radius );
        draw->setColor(color);
        window.draw(*draw);
    }
    //
}
//
void Renderer::Shutdown(){
    window.close();
    if(thread.joinable()){
        thread.join();
    }
}
//
