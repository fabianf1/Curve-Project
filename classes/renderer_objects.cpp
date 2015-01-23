// Contains functions and constructors for the Renderer Objects class
// Needed Header
#include "renderer_objects.h"
// Constructor
Renderer_Objects::Renderer_Objects(const Config &config): font(LoadFont("fontdata")) {
    // Set variables
    font_title_size=50;
    font_size=30;
    g_font_title_size=25;
    g_font_size=15;
    vector_length=0;
    // Initialize Main Menu
    Init_Main_Menu(config);
    Init_Game_Setup(config);
    Init_Game(config);
}
// Main Menu Init
void Renderer_Objects::Init_Main_Menu(const Config &config){
    // Title
    m_title.setString("Curve Project");
    m_title.setFont(font);
    m_title.setCharacterSize(font_title_size);
    m_title.setColor(sf::Color::White);
    m_title.setStyle(sf::Text::Bold);
    m_title.setPosition( (config.window_width/2)-(m_title.getLocalBounds().width/2) ,10); // Middle top
    // Start
    m_create.setButton("Create Game",font,font_size,sf::Color::Red,sf::Color::Yellow,sf::Text::Regular,sf::Text::Regular);
    m_create.setPosition( 50 , config.window_height/2); // Left middle
    // Server
    m_join.setButton("Join Game",font,font_size,sf::Color::Red,sf::Color::Yellow,sf::Text::Regular,sf::Text::Regular);
    m_join.setPosition( 50 , config.window_height/2+40); // Left ~middle
    // Quit
    m_quit.setButton("Quit",font,font_size,sf::Color::Red,sf::Color::Yellow,sf::Text::Regular,sf::Text::Regular);
    m_quit.setPosition( 50 , config.window_height/2+80); // Left ~middle
    // Prompt
    m_ip_prompt.setPrompt("Enter IP:",font,font_size,config.window_width/2,config.window_height/2);
    #ifdef DEBUG
    m_ip_prompt.setString(sf::IpAddress::getLocalAddress().toString());
    #endif // DEBUG
}
// Game Setup Init
void Renderer_Objects::Init_Game_Setup(const Config &config){
    // Title
    s_title.setString("Game Setup");
    s_title.setFont(font);
    s_title.setCharacterSize(font_title_size);
    s_title.setColor(sf::Color::White);
    s_title.setStyle(sf::Text::Bold);
    s_title.setPosition( (config.window_width/2)-(s_title.getLocalBounds().width/2) ,10); // Middle Top
    // Name
    s_name.setString("Name");
    s_name.setFont(font);
    s_name.setCharacterSize(font_size);
    s_name.setColor(sf::Color::White);
    s_name.setStyle(sf::Text::Bold);
    s_name.setPosition(40 , 150); // Left Semi-top
    // Left
    s_left.setString("Left");
    s_left.setFont(font);
    s_left.setCharacterSize(font_size);
    s_left.setColor(sf::Color::White);
    s_left.setStyle(sf::Text::Bold);
    s_left.setPosition(200 , 150);
    // Right
    s_right.setString("Right");
    s_right.setFont(font);
    s_right.setCharacterSize(font_size);
    s_right.setColor(sf::Color::White);
    s_right.setStyle(sf::Text::Bold);
    s_right.setPosition(300 , 150);
    // Player things
    //
    s_add.setButton("Add",font,font_size,sf::Color::Red,sf::Color::Yellow,sf::Text::Regular,sf::Text::Italic);
    s_add.setPosition(40,150+30*1.5);

    // Vectors initialized when a player is added!
    //
    s_server.setButton("Start Server",font,font_size,sf::Color::Red,sf::Color::Yellow,sf::Text::Regular,sf::Text::Italic);
    s_server.setPosition(40,config.window_height-50);
    // Options are on the right side
    s_options.setString("Options");
    s_options.setFont(font);
    s_options.setCharacterSize(font_size*0.75);
    s_options.setColor(sf::Color::White);
    s_options.setStyle(sf::Text::Bold);
    s_options.setPosition(750 , 150);
    //
    s_maxpoints.setString("Max Points:");
    s_maxpoints.setFont(font);
    s_maxpoints.setCharacterSize(font_size*0.75);
    s_maxpoints.setColor(sf::Color::White);
    //s_maxpoints.setStyle(sf::Text::Regular);
    s_maxpoints.setPosition(670 , 200);
    //
    s_max10.setButton("10",font,font_size*0.75,sf::Color::Red,sf::Color::Yellow,sf::Text::Regular,sf::Text::Italic);
    s_max10.setPosition(830,200);
    //
    s_max20.setButton("20",font,font_size*0.75,sf::Color::Red,sf::Color::Yellow,sf::Text::Regular,sf::Text::Italic);
    s_max20.setPosition(870,200);
    //
    s_max40.setButton("40",font,font_size*0.75,sf::Color::Red,sf::Color::Yellow,sf::Text::Regular,sf::Text::Italic);
    s_max40.setPosition(910,200);
    //
    s_powerups.setString("Powerups:");
    s_powerups.setFont(font);
    s_powerups.setCharacterSize(font_size*0.75);
    s_powerups.setColor(sf::Color::White);
    s_powerups.setPosition(670 , 240);
    //
    s_powerupon.setButton("On",font,font_size*0.75,sf::Color::Red,sf::Color::Yellow,sf::Text::Regular,sf::Text::Italic);
    s_powerupon.setPosition(830,240);
    //
    s_powerupoff.setButton("Off",font,font_size*0.75,sf::Color::Red,sf::Color::Yellow,sf::Text::Regular,sf::Text::Italic);
    s_powerupoff.setPosition(880,240);
    //
    s_countdown.setString("Countdown:");
    s_countdown.setFont(font);
    s_countdown.setCharacterSize(font_size*0.75);
    s_countdown.setColor(sf::Color::White);
    s_countdown.setPosition(670 , 280);
    //
    s_countdownon.setButton("On",font,font_size*0.75,sf::Color::Red,sf::Color::Yellow,sf::Text::Regular,sf::Text::Italic);
    s_countdownon.setPosition(830,280);
    //
    s_countdownoff.setButton("Off",font,font_size*0.75,sf::Color::Red,sf::Color::Yellow,sf::Text::Regular,sf::Text::Italic);
    s_countdownoff.setPosition(880,280);
    // Buttons :D
    s_start.setButton("Start Game",font,font_size,sf::Color::Red,sf::Color::Yellow,sf::Text::Regular,sf::Text::Italic);
    s_start.setPosition(config.window_width/2-s_start.getLocalBounds().width/2,config.window_height-100); // Middle Bottom
    //
    s_quit.setButton("Quit",font,font_size,sf::Color::Red,sf::Color::Yellow,sf::Text::Regular,sf::Text::Italic);
    s_quit.setPosition(config.window_width/2-s_quit.getLocalBounds().width/2,config.window_height-50);// Middle More-Bottom
}
//
void Renderer_Objects::Init_Game(const Config &config){
    // Walls
    // Left
    g_leftwall.setFillColor(config.wallcolor);
    g_leftwall.setSize(sf::Vector2f(config.wallwidth, config.window_height));
    // Top
    g_topwall.setSize(sf::Vector2f(config.window_width-config.statuswidth-2*config.wallwidth,config.wallwidth));
    g_topwall.setPosition(config.wallwidth,0);
    // Right
    g_rightwall.setSize(sf::Vector2f(config.wallwidth, config.window_height));
    g_rightwall.setPosition(config.window_width-config.wallwidth-config.statuswidth,0);
    // Bottom
    g_bottomwall.setSize(sf::Vector2f(config.window_width-config.statuswidth-2*config.wallwidth,config.wallwidth));
    g_bottomwall.setPosition(config.wallwidth,config.window_height-config.wallwidth);
    // Scores title
    g_scores.setFont(font);
    g_scores.setString("Scores");
    g_scores.setCharacterSize(font_size);
    g_scores.setPosition( config.window_width-config.statuswidth/2-g_scores.getLocalBounds().width/2,10);
    g_scores.setColor(sf::Color::White);
    g_scores.setStyle(sf::Text::Bold);
    // Names and score initialized when needed
    // Round
    g_round[0].setCharacterSize(font_size*0.75);
    g_round[0].setFont(font);
    g_round[0].setColor(sf::Color::Yellow);
    g_round[0].setString("Round:");
    g_round[0].setPosition(config.window_width-config.statuswidth+5,config.window_height/2);
    g_round[1].setCharacterSize(font_size*0.75);
    g_round[1].setFont(font);
    g_round[1].setColor(sf::Color::Yellow);
    g_round[1].setPosition(config.window_width-config.statuswidth+100,config.window_height/2);
    // FPS
    g_fps[0].setCharacterSize(font_size*0.75);
    g_fps[0].setFont(font);
    g_fps[0].setColor(sf::Color::Yellow);
    g_fps[0].setString("FPS:");
    g_fps[0].setPosition(config.window_width-config.statuswidth+5,config.window_height/2+30);
    g_fps[1].setCharacterSize(font_size*0.75);
    g_fps[1].setFont(font);
    g_fps[1].setColor(sf::Color::Yellow);
    g_fps[1].setPosition(config.window_width-config.statuswidth+100,config.window_height/2+30);
    // Packet
    g_packet[0].setCharacterSize(font_size*0.75);
    g_packet[0].setFont(font);
    g_packet[0].setColor(sf::Color::Yellow);
    g_packet[0].setString("PPS:");
    g_packet[0].setPosition(config.window_width-config.statuswidth+5,config.window_height/2+60);
    g_packet[1].setCharacterSize(font_size*0.75);
    g_packet[1].setFont(font);
    g_packet[1].setColor(sf::Color::Yellow);
    g_packet[1].setPosition(config.window_width-config.statuswidth+100,config.window_height/2+60);
    // Quit Button
    g_quit.setButton("Quit",font,font_size,sf::Color::Red,sf::Color::Yellow,sf::Text::Bold,sf::Text::Bold);
    g_quit.setPosition(config.window_width-config.statuswidth/2-g_quit.getLocalBounds().width/2,config.window_height-50);
    // End round message
    g_end_round_message.setFont(font);
    g_end_round_message.setCharacterSize(50);
    g_end_round_message.setStyle(sf::Text::Bold);
    // Countdown
    g_countdown.setFont(font);
    g_countdown.setCharacterSize(50);
    g_countdown.setStyle(sf::Text::Bold);
}
// Vector things
void Renderer_Objects::Sync_Players(const Config &config,const std::vector<Player> &player){
    //
    if(vector_length<player.size()){
        for(unsigned int i=0;i<(player.size()-vector_length);i++){
            Add_Player(config,player);
        }
    }
    else if(vector_length>player.size()){
        for(unsigned int i=0;i<(vector_length-player.size());i++){
            vector_length--;
            s_names.erase(s_names.end());
            s_lbutton.erase(s_lbutton.end());
            s_rbutton.erase(s_rbutton.end());
            s_status.erase(s_status.end());
            s_kick.erase(s_kick.end());
            g_names.erase(g_names.end());
            g_score.erase(g_score.end());
        }
    }
    // Synchronize
    for(unsigned int i=0;i<vector_length;i++){
        s_names[i].setString(player[i].name);
        s_names[i].setColors(player[i].color,player[i].color);
        s_names[i].setPosition(40,195+(i)*45);
        //
        s_lbutton[i].setString(getKeyName(player[i].keyL));
        s_lbutton[i].setColors(player[i].color,player[i].color);
        s_lbutton[i].setPosition(200,195+(i)*45);
        //
        s_rbutton[i].setString(getKeyName(player[i].keyR));
        s_rbutton[i].setColors(player[i].color,player[i].color);
        s_rbutton[i].setPosition(300,195+(i)*45);
        //
        if(!player[i].local&&!player[i].server&&!player[i].ready){
            s_status[i].setString("Not ready");
        }
        else if(!player[i].local&&!player[i].server&&player[i].ready){
            s_status[i].setString("Ready");
        }
        else if(player[i].server){
            s_status[i].setString("Server");
        }
        //
        s_status[i].setColor(player[i].color);
        s_status[i].setPosition(200,195+(i)*45);
        //
        s_kick[i].setColors(player[i].color,player[i].color);
        s_kick[i].setPosition(400,195+(i)*45);
        //
        g_names[i].setString(player[i].name);
        g_names[i].setColor(player[i].color);
        g_names[i].setPosition(config.window_width-config.statuswidth+5,50+i*30);
        //
        g_score[i].setString(int2string(player[i].points));
        g_score[i].setColor(player[i].color);
        g_score[i].setPosition(config.window_width-30,50+i*30);
    }
    // Set position of add button
    s_add.setPosition(40,150+30*1.5+45*vector_length);
}
//
void Renderer_Objects::Add_Player(const Config &config,const std::vector<Player> &player){
    // Game Setup
    s_names.emplace_back(player[vector_length].name,font,font_size,player[vector_length].color,player[vector_length].color,sf::Text::Regular,sf::Text::Italic,8);
    s_names[vector_length].setPosition(40,195+(vector_length)*45 );;
    //
    s_lbutton.emplace_back("None",font,font_size,player[vector_length].color,player[vector_length].color,sf::Text::Regular,sf::Text::Italic);
    s_lbutton[vector_length].setPosition(200,195+(vector_length)*45 );
    //
    s_rbutton.emplace_back("None",font,font_size,player[vector_length].color,player[vector_length].color,sf::Text::Regular,sf::Text::Italic);
    s_rbutton[vector_length].setPosition(300,195+(vector_length)*45 );
    //
    s_status.emplace_back("W00T",font,font_size);
    s_status[vector_length].setColor(sf::Color::White);
    s_status[vector_length].setPosition(670 , 240);
    //
    s_kick.emplace_back("Kick!",font,font_size,player[vector_length].color,player[vector_length].color,sf::Text::Regular,sf::Text::Italic);
    s_kick[vector_length].setPosition(400,195+(vector_length)*45 );
    // Game
    // Name
    g_names.emplace_back(player[vector_length].name,font,font_size*0.75);
    g_names[vector_length].setColor(player[vector_length].color);
    g_names[vector_length].setPosition(config.window_width-config.statuswidth+5,50+vector_length*30);
    // Score
    g_score.emplace_back("0",font,font_size*0.75);
    g_score[vector_length].setColor(player[vector_length].color);
    g_score[vector_length].setPosition(config.window_width-30,50+vector_length*30);
    //
    vector_length++;
}
//
void Renderer_Objects::setOptions(const Game &game){
    //
    // Points
    if(game.maxpoints==10){
        s_max10.setActive(true);
        s_max20.setActive(false);
        s_max40.setActive(false);
    }
    else if(game.maxpoints==20){
        s_max10.setActive(false);
        s_max20.setActive(true);
        s_max40.setActive(false);
    }
    else if(game.maxpoints==40){
        s_max10.setActive(false);
        s_max20.setActive(false);
        s_max40.setActive(true);
    }
    //
    // Powerup
    if(game.powerup_enabled){
        s_powerupon.setActive(true);
        s_powerupoff.setActive(false);
    }
    else{
        s_powerupon.setActive(false);
        s_powerupoff.setActive(true);
    }
    // Countdown
    if(game.countdown_enabled){
        s_countdownon.setActive(true);
        s_countdownoff.setActive(false);
    }
    else{
        s_countdownon.setActive(false);
        s_countdownoff.setActive(true);
    }
}
// Font Loader
sf::Font Renderer_Objects::LoadFont(const std::string& name){
    HRSRC rsrcData = FindResource(NULL, name.c_str(), RT_RCDATA);
    if (!rsrcData)
        throw std::runtime_error("Failed to find resource.");

    DWORD rsrcDataSize = SizeofResource(NULL, rsrcData);
    if (rsrcDataSize <= 0)
        throw std::runtime_error("Size of resource is 0.");

    HGLOBAL grsrcData = LoadResource(NULL, rsrcData);
    if (!grsrcData)
        throw std::runtime_error("Failed to load resource.");

    LPVOID firstByte = LockResource(grsrcData);
    if (!firstByte)
        throw std::runtime_error("Failed to lock resource.");

    sf::Font font;
    if (!font.loadFromMemory(firstByte, rsrcDataSize))
        throw std::runtime_error("Failed to load font from memory.");

    return font;
}
//
const char* Renderer_Objects::getKeyName( const sf::Keyboard::Key &key ) {
    switch( key ) {
    default:
    case sf::Keyboard::Unknown:
        return "None";
    case sf::Keyboard::A:
        return "A";
    case sf::Keyboard::B:
        return "B";
    case sf::Keyboard::C:
        return "C";
    case sf::Keyboard::D:
        return "D";
    case sf::Keyboard::E:
        return "E";
    case sf::Keyboard::F:
        return "F";
    case sf::Keyboard::G:
        return "G";
    case sf::Keyboard::H:
        return "H";
    case sf::Keyboard::I:
        return "I";
    case sf::Keyboard::J:
        return "J";
    case sf::Keyboard::K:
        return "K";
    case sf::Keyboard::L:
        return "L";
    case sf::Keyboard::M:
        return "M";
    case sf::Keyboard::N:
        return "N";
    case sf::Keyboard::O:
        return "O";
    case sf::Keyboard::P:
        return "P";
    case sf::Keyboard::Q:
        return "Q";
    case sf::Keyboard::R:
        return "R";
    case sf::Keyboard::S:
        return "S";
    case sf::Keyboard::T:
        return "T";
    case sf::Keyboard::U:
        return "U";
    case sf::Keyboard::V:
        return "V";
    case sf::Keyboard::W:
        return "W";
    case sf::Keyboard::X:
        return "X";
    case sf::Keyboard::Y:
        return "Y";
    case sf::Keyboard::Z:
        return "Z";
    case sf::Keyboard::Num0:
        return "Num0";
    case sf::Keyboard::Num1:
        return "Num1";
    case sf::Keyboard::Num2:
        return "Num2";
    case sf::Keyboard::Num3:
        return "Num3";
    case sf::Keyboard::Num4:
        return "Num4";
    case sf::Keyboard::Num5:
        return "Num5";
    case sf::Keyboard::Num6:
        return "Num6";
    case sf::Keyboard::Num7:
        return "Num7";
    case sf::Keyboard::Num8:
        return "Num8";
    case sf::Keyboard::Num9:
        return "Num9";
    case sf::Keyboard::Escape:
        return "Escape";
    case sf::Keyboard::LControl:
        return "LControl";
    case sf::Keyboard::LShift:
        return "LShift";
    case sf::Keyboard::LAlt:
        return "LAlt";
    case sf::Keyboard::LSystem:
        return "LSystem";
    case sf::Keyboard::RControl:
        return "RControl";
    case sf::Keyboard::RShift:
        return "RShift";
    case sf::Keyboard::RAlt:
        return "RAlt";
    case sf::Keyboard::RSystem:
        return "RSystem";
    case sf::Keyboard::Menu:
        return "Menu";
    case sf::Keyboard::LBracket:
        return "LBracket";
    case sf::Keyboard::RBracket:
        return "RBracket";
    case sf::Keyboard::SemiColon:
        return "SemiColon";
    case sf::Keyboard::Comma:
        return "Comma";
    case sf::Keyboard::Period:
        return "Period";
    case sf::Keyboard::Quote:
        return "Quote";
    case sf::Keyboard::Slash:
        return "Slash";
    case sf::Keyboard::BackSlash:
        return "BackSlash";
    case sf::Keyboard::Tilde:
        return "Tilde";
    case sf::Keyboard::Equal:
        return "Equal";
    case sf::Keyboard::Dash:
        return "Dash";
    case sf::Keyboard::Space:
        return "Space";
    case sf::Keyboard::Return:
        return "Return";
    case sf::Keyboard::BackSpace:
        return "BackSpace";
    case sf::Keyboard::Tab:
        return "Tab";
    case sf::Keyboard::PageUp:
        return "PageUp";
    case sf::Keyboard::PageDown:
        return "PageDown";
    case sf::Keyboard::End:
        return "End";
    case sf::Keyboard::Home:
        return "Home";
    case sf::Keyboard::Insert:
        return "Insert";
    case sf::Keyboard::Delete:
        return "Delete";
    case sf::Keyboard::Add:
        return "Add";
    case sf::Keyboard::Subtract:
        return "Subtract";
    case sf::Keyboard::Multiply:
        return "Multiply";
    case sf::Keyboard::Divide:
        return "Divide";
    case sf::Keyboard::Left:
        return "Left";
    case sf::Keyboard::Right:
        return "Right";
    case sf::Keyboard::Up:
        return "Up";
    case sf::Keyboard::Down:
        return "Down";
    case sf::Keyboard::Numpad0:
        return "Numpad0";
    case sf::Keyboard::Numpad1:
        return "Numpad1";
    case sf::Keyboard::Numpad2:
        return "Numpad2";
    case sf::Keyboard::Numpad3:
        return "Numpad3";
    case sf::Keyboard::Numpad4:
        return "Numpad4";
    case sf::Keyboard::Numpad5:
        return "Numpad5";
    case sf::Keyboard::Numpad6:
        return "Numpad6";
    case sf::Keyboard::Numpad7:
        return "Numpad7";
    case sf::Keyboard::Numpad8:
        return "Numpad8";
    case sf::Keyboard::Numpad9:
        return "Numpad9";
    case sf::Keyboard::F1:
        return "F1";
    case sf::Keyboard::F2:
        return "F2";
    case sf::Keyboard::F3:
        return "F3";
    case sf::Keyboard::F4:
        return "F4";
    case sf::Keyboard::F5:
        return "F5";
    case sf::Keyboard::F6:
        return "F6";
    case sf::Keyboard::F7:
        return "F7";
    case sf::Keyboard::F8:
        return "F8";
    case sf::Keyboard::F9:
        return "F9";
    case sf::Keyboard::F10:
        return "F10";
    case sf::Keyboard::F11:
        return "F11";
    case sf::Keyboard::F12:
        return "F12";
    case sf::Keyboard::F13:
        return "F13";
    case sf::Keyboard::F14:
        return "F14";
    case sf::Keyboard::F15:
        return "F15";
    case sf::Keyboard::Pause:
        return "Pause";
    }
}
