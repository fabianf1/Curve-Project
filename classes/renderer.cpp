// Contains functions and constructors for the Renderer class
// Needed Header
#include "renderer.h"
//
void Renderer::Start(const Config &config,Game &game,const std::vector<Player> &player){
    // Window
    window.create(sf::VideoMode(config.window_width, config.window_height), config.title, config.window_style, sf::ContextSettings(24,8,config.window_antialising));
    window.setIcon(202,202,config.icon.getPixelsPtr());
    window.clear(config.window_backgroundcolor);
    window.display();
    // Limit fps
    window.setFramerateLimit(config.fps);
    // Start Render thread
    window.setActive(false);
    thread = std::thread(&Renderer::Thread,this,std::cref(config),std::ref(game),std::cref(player));
    // Done
}
// Render Thread Function
void Renderer::Thread(const Config &config,Game &game,const std::vector<Player> &player){
    std::cout << "Render Thread Started" << std::endl;
    // Start main loop
    while(window.isOpen()){
        window.clear(config.window_backgroundcolor);
        // Lock the mode mutex. Not rendering while mode is changing solves font problems
        game.mode_mutex.lock();
        // Render the things
        if(game.mode==Game::Mode::Main_Menu){
            Main_Menu();
        }
        else if(game.mode==Game::Mode::Setup){
            Setup(config,game,player);
        }
        else if(game.mode==Game::Mode::Play){
            Play(config,game,player);
            if(game.powerup_enabled){PowerUp(config,game,player);}
        }
        // Unlock
        game.mode_mutex.unlock();
        //
        window.display();
        game.frame++;
        // FPS
        if(game.frame%(config.fps)==0){
            sf::Time temp=game.fps_clock.restart();
            //std::cout << temp.asSeconds() << std::endl;
            objects.g_fps[1].setString(int2string( (1.0/temp.asSeconds())*(config.fps) ));
        }
    }
    std::cout << "Render Thread Stopped" << std::endl;
}
//
void Renderer::Main_Menu(){
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
void Renderer::Setup(const Config &config,const Game &game,const std::vector<Player> &player){
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
        // Only display kick when local or server and if server then always keep one.
        if(!game.client[1]&& ( (game.server[1] && (i!=0) ) || (!game.server[1]) ) ){window.draw(objects.s_kick[i]);}
    }
    // More
    if(objects.vector_length<6&&!game.client[1]){
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
void Renderer::Play(const Config &config,const Game &game,const std::vector<Player> &player){
    // Draw Player Things
    for(unsigned int i=0;i<player.size();i++){
        //player[i].Draw(window);
        if(!game.darkness){
            window.draw(player[i].line);
            if(!player[i].rightangle){
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
                    if( (xc-player[k].x)*(xc-player[k].x) + (yc-player[k].y)*(yc-player[k].y) < (config.darkness_radius*config.darkness_radius)/4 ){
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
                if(k==i || ( (player[i].x-player[k].x)*(player[i].x-player[k].x) + (player[i].y-player[k].y)*(player[i].y-player[k].y) < (config.darkness_radius*config.darkness_radius)/4 ) ){
                    if(!player[i].rightangle){
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
    window.draw(objects.g_fps[0]);
    window.draw(objects.g_fps[1]);
    //window.draw(objects.g_packet[0]);
    //window.draw(objects.g_packet[1]);
    window.draw(objects.g_quit);
    if(game.round_finished&&game.end_message_set){window.draw(objects.g_end_round_message);}
    if(game.countdown_int>0){window.draw(objects.g_countdown);}
}
//
void Renderer::PowerUp(const Config &config,const Game &game,const std::vector<Player> &player){
    for(unsigned int i=0;i<game.powerup_field.size();i++){
        // Check if withing FOV
        if(game.darkness){
            bool draw=false;
            int diffx;
            int diffy;
            for(unsigned int j=0;j<player.size();j++){
                diffx=(player[j].x-game.powerup_field[i].x);
                diffy=(player[j].y-game.powerup_field[i].y);
                if( diffx*diffx+diffy*diffy < (config.darkness_radius*config.darkness_radius)/4 ){
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
        // Darkness
        else if(game.powerup_field[i].type==Powerup::Type::Darkness){
            draw=&sprite.darknessblue;
        }
        // Gap
        else if(game.powerup_field[i].type==Powerup::Type::Gap){
            draw=&sprite.gapred;
        }
        // Bomb
        else if(game.powerup_field[i].type==Powerup::Type::Bomb){
            draw=&sprite.bombblue;
        }
        // Sine
        else if(game.powerup_field[i].type==Powerup::Type::Sine){
            draw=&sprite.sinered;
        }
        else{
            std::cout << "Error! No sprite!" << std::endl;
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
