// Contains functions and constructors for the Renderer Objects class
// Needed Header
#include "rendererObjects.h"
// Constructor
RendererObjects::RendererObjects(const Config &config): font(loadFont("fontdata")) {
    // Set variables
    fontTitleSize=50;
    fontSize=30;
    g_fontTitleSize=25;
    g_fontSize=15;
    vectorLength=0;
    // initialize Main Menu
    initMainMenu(config);
    initGameSetup(config);
    initGame(config);
}
// Main Menu Init
void RendererObjects::initMainMenu(const Config &config){
    // Title
    m_title.setString("Curve Project");
    m_title.setFont(font);
    m_title.setCharacterSize(fontTitleSize);
    m_title.setColor(sf::Color::White);
    m_title.setStyle(sf::Text::Bold);
    m_title.setPosition( (config.windowWidth/2)-(m_title.getLocalBounds().width/2) ,10); // Middle top
    // start
    m_create.setButton("Create Game",font,fontSize,sf::Color::Red,sf::Color::Yellow,sf::Text::Regular,sf::Text::Regular);
    m_create.setPosition( 50 , config.windowHeight/2); // Left middle
    // Server
    m_join.setButton("Join Game",font,fontSize,sf::Color::Red,sf::Color::Yellow,sf::Text::Regular,sf::Text::Regular);
    m_join.setPosition( 50 , config.windowHeight/2+40); // Left ~middle
    // Quit
    m_quit.setButton("Quit",font,fontSize,sf::Color::Red,sf::Color::Yellow,sf::Text::Regular,sf::Text::Regular);
    m_quit.setPosition( 50 , config.windowHeight/2+80); // Left ~middle
    // Prompt
    m_ipPrompt.setPrompt("Enter IP:",font,fontSize,config.windowWidth/2,config.windowHeight/2);
    #ifdef DEBUG
    m_ipPrompt.setString(sf::IpAddress::getLocalAddress().toString());
    #endif // DEBUG
}
// Game setup Init
void RendererObjects::initGameSetup(const Config &config){
    // Title
    s_title.setString("Game setup");
    s_title.setFont(font);
    s_title.setCharacterSize(fontTitleSize);
    s_title.setColor(sf::Color::White);
    s_title.setStyle(sf::Text::Bold);
    s_title.setPosition( (config.windowWidth/2)-(s_title.getLocalBounds().width/2) ,10); // Middle Top
    // Name
    s_name.setString("Name");
    s_name.setFont(font);
    s_name.setCharacterSize(fontSize);
    s_name.setColor(sf::Color::White);
    s_name.setStyle(sf::Text::Bold);
    s_name.setPosition(40 , 150); // Left Semi-top
    // Left
    s_left.setString("Left");
    s_left.setFont(font);
    s_left.setCharacterSize(fontSize);
    s_left.setColor(sf::Color::White);
    s_left.setStyle(sf::Text::Bold);
    s_left.setPosition(200 , 150);
    // Right
    s_right.setString("Right");
    s_right.setFont(font);
    s_right.setCharacterSize(fontSize);
    s_right.setColor(sf::Color::White);
    s_right.setStyle(sf::Text::Bold);
    s_right.setPosition(300 , 150);
    // Player things
    //
    s_add.setButton("Add",font,fontSize,sf::Color::Red,sf::Color::Yellow,sf::Text::Regular,sf::Text::Italic);
    s_add.setPosition(40,150+30*1.5);

    // Vectors initialized when a player is added!
    //
    s_server.setButton("Start Server",font,fontSize,sf::Color::Red,sf::Color::Yellow,sf::Text::Regular,sf::Text::Italic);
    s_server.setPosition(40,config.windowHeight-50);
    // Options are on the right side
    s_options.setString("Options");
    s_options.setFont(font);
    s_options.setCharacterSize(fontSize*0.75);
    s_options.setColor(sf::Color::White);
    s_options.setStyle(sf::Text::Bold);
    s_options.setPosition(750 , 150);
    //
    s_maxPoints.setString("Max Points:");
    s_maxPoints.setFont(font);
    s_maxPoints.setCharacterSize(fontSize*0.75);
    s_maxPoints.setColor(sf::Color::White);
    //s_maxPoints.setStyle(sf::Text::Regular);
    s_maxPoints.setPosition(670 , 200);
    //
    s_max10.setButton("10",font,fontSize*0.75,sf::Color::Red,sf::Color::Yellow,sf::Text::Regular,sf::Text::Italic);
    s_max10.setPosition(840,200);
    //
    s_max20.setButton("20",font,fontSize*0.75,sf::Color::Red,sf::Color::Yellow,sf::Text::Regular,sf::Text::Italic);
    s_max20.setPosition(880,200);
    //
    s_max40.setButton("40",font,fontSize*0.75,sf::Color::Red,sf::Color::Yellow,sf::Text::Regular,sf::Text::Italic);
    s_max40.setPosition(920,200);
    //
    s_powerups.setString("Powerups:");
    s_powerups.setFont(font);
    s_powerups.setCharacterSize(fontSize*0.75);
    s_powerups.setColor(sf::Color::White);
    s_powerups.setPosition(670 , 240);
    //
    s_powerupOn.setButton("On",font,fontSize*0.75,sf::Color::Red,sf::Color::Yellow,sf::Text::Regular,sf::Text::Italic);
    s_powerupOn.setPosition(840,240);
    //
    s_powerupOff.setButton("Off",font,fontSize*0.75,sf::Color::Red,sf::Color::Yellow,sf::Text::Regular,sf::Text::Italic);
    s_powerupOff.setPosition(890,240);
    //
    s_countdown.setString("Countdown:");
    s_countdown.setFont(font);
    s_countdown.setCharacterSize(fontSize*0.75);
    s_countdown.setColor(sf::Color::White);
    s_countdown.setPosition(670 , 280);
    //
    s_countdownOn.setButton("On",font,fontSize*0.75,sf::Color::Red,sf::Color::Yellow,sf::Text::Regular,sf::Text::Italic);
    s_countdownOn.setPosition(840,280);
    //
    s_countdownOff.setButton("Off",font,fontSize*0.75,sf::Color::Red,sf::Color::Yellow,sf::Text::Regular,sf::Text::Italic);
    s_countdownOff.setPosition(890,280);
    //
    s_multiplePlayers.setString("Multiple players:");
    s_multiplePlayers.setFont(font);
    s_multiplePlayers.setCharacterSize(fontSize*0.75);
    s_multiplePlayers.setColor(sf::Color::White);
    s_multiplePlayers.setPosition(670 , 280);
    //
    s_multiplePlayersOn.setButton("On",font,fontSize*0.75,sf::Color::Red,sf::Color::Yellow,sf::Text::Regular,sf::Text::Italic);
    s_multiplePlayersOn.setPosition(840,280);
    //
    s_multiplePlayersOff.setButton("Off",font,fontSize*0.75,sf::Color::Red,sf::Color::Yellow,sf::Text::Regular,sf::Text::Italic);
    s_multiplePlayersOff.setPosition(890,280);
    // Buttons :D
    s_start.setButton("Start Game",font,fontSize,sf::Color::Red,sf::Color::Yellow,sf::Text::Regular,sf::Text::Italic);
    s_start.setPosition(config.windowWidth/2-s_start.getLocalBounds().width/2,config.windowHeight-100); // Middle Bottom
    //
    s_quit.setButton("Quit",font,fontSize,sf::Color::Red,sf::Color::Yellow,sf::Text::Regular,sf::Text::Italic);
    s_quit.setPosition(config.windowWidth/2-s_quit.getLocalBounds().width/2,config.windowHeight-50);// Middle More-Bottom
}
//
void RendererObjects::initGame(const Config &config){
    // Walls
    // Left
    g_leftWall.setFillColor(config.wallColor);
    g_leftWall.setSize(sf::Vector2f(config.wallWidth, config.windowHeight));
    // Top
    g_topWall.setSize(sf::Vector2f(config.windowWidth-config.statusWidth-2*config.wallWidth,config.wallWidth));
    g_topWall.setPosition(config.wallWidth,0);
    // Right
    g_rightWall.setSize(sf::Vector2f(config.wallWidth, config.windowHeight));
    g_rightWall.setPosition(config.windowWidth-config.wallWidth-config.statusWidth,0);
    // Bottom
    g_bottomWall.setSize(sf::Vector2f(config.windowWidth-config.statusWidth-2*config.wallWidth,config.wallWidth));
    g_bottomWall.setPosition(config.wallWidth,config.windowHeight-config.wallWidth);
    // Scores title
    g_scoreTitle.setFont(font);
    g_scoreTitle.setString("Scores");
    g_scoreTitle.setCharacterSize(fontSize);
    g_scoreTitle.setPosition( config.windowWidth-config.statusWidth/2-g_scoreTitle.getLocalBounds().width/2,10);
    g_scoreTitle.setColor(sf::Color::White);
    g_scoreTitle.setStyle(sf::Text::Bold);
    // Names and score initialized when needed
    // Round
    g_round[0].setCharacterSize(fontSize*0.75);
    g_round[0].setFont(font);
    g_round[0].setColor(sf::Color::Yellow);
    g_round[0].setString("Round:");
    g_round[0].setPosition(config.windowWidth-config.statusWidth+5,config.windowHeight/2);
    g_round[1].setCharacterSize(fontSize*0.75);
    g_round[1].setFont(font);
    g_round[1].setColor(sf::Color::Yellow);
    g_round[1].setPosition(config.windowWidth-config.statusWidth+100,config.windowHeight/2);
    // FPS
    g_fps[0].setCharacterSize(fontSize*0.75);
    g_fps[0].setFont(font);
    g_fps[0].setColor(sf::Color::Yellow);
    g_fps[0].setString("FPS:");
    g_fps[0].setPosition(config.windowWidth-config.statusWidth+5,config.windowHeight/2+30);
    g_fps[1].setCharacterSize(fontSize*0.75);
    g_fps[1].setFont(font);
    g_fps[1].setColor(sf::Color::Yellow);
    g_fps[1].setPosition(config.windowWidth-config.statusWidth+100,config.windowHeight/2+30);
    // Packet
    g_packet[0].setCharacterSize(fontSize*0.75);
    g_packet[0].setFont(font);
    g_packet[0].setColor(sf::Color::Yellow);
    g_packet[0].setString("PPS:");
    g_packet[0].setPosition(config.windowWidth-config.statusWidth+5,config.windowHeight/2+60);
    g_packet[1].setCharacterSize(fontSize*0.75);
    g_packet[1].setFont(font);
    g_packet[1].setColor(sf::Color::Yellow);
    g_packet[1].setPosition(config.windowWidth-config.statusWidth+100,config.windowHeight/2+60);
    // Quit Button
    g_quit.setButton("Quit",font,fontSize,sf::Color::Red,sf::Color::Yellow,sf::Text::Bold,sf::Text::Bold);
    g_quit.setPosition(config.windowWidth-config.statusWidth/2-g_quit.getLocalBounds().width/2,config.windowHeight-50);
    // End round message
    g_endRoundMessage.setFont(font);
    g_endRoundMessage.setCharacterSize(50);
    g_endRoundMessage.setStyle(sf::Text::Bold);
    // Countdown
    g_countdown.setFont(font);
    g_countdown.setCharacterSize(50);
    g_countdown.setStyle(sf::Text::Bold);
}
// Vector things
void RendererObjects::syncPlayers(const Config &config,const std::vector<Player> &player){
    //
    if(vectorLength<player.size()){
        for(unsigned int i=0;i<(player.size()-vectorLength);i++){
            addPlayer(config,player);
        }
    }
    else if(vectorLength>player.size()){
        for(unsigned int i=0;i<(vectorLength-player.size());i++){
            vectorLength--;
            s_names.erase(s_names.end());
            s_leftButton.erase(s_leftButton.end());
            s_rightButton.erase(s_rightButton.end());
            s_status.erase(s_status.end());
            s_kick.erase(s_kick.end());
            g_names.erase(g_names.end());
            g_scores.erase(g_scores.end());
        }
    }
    // Synchronize
    for(unsigned int i=0;i<vectorLength&&i<player.size();i++){
        s_names[i].setString(player[i].name);
        s_names[i].setColors(player[i].color,player[i].color);
        s_names[i].setPosition(40,195+(i)*45);
        //
        s_leftButton[i].setString(getKeyName(player[i].keyL));
        s_leftButton[i].setColors(player[i].color,player[i].color);
        s_leftButton[i].setPosition(200,195+(i)*45);
        //
        s_rightButton[i].setString(getKeyName(player[i].keyR));
        s_rightButton[i].setColors(player[i].color,player[i].color);
        s_rightButton[i].setPosition(300,195+(i)*45);
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
        g_names[i].setPosition(config.windowWidth-config.statusWidth+5,50+i*30);
        //
        g_scores[i].setString(int2str(player[i].points));
        g_scores[i].setColor(player[i].color);
        g_scores[i].setPosition(config.windowWidth-30,50+i*30);
    }
    // Set position of add button
    s_add.setPosition(40,150+30*1.5+45*vectorLength);
}
//
void RendererObjects::addPlayer(const Config &config,const std::vector<Player> &player){
    // Game setup
    s_names.emplace_back(player[vectorLength].name,font,fontSize,player[vectorLength].color,player[vectorLength].color,sf::Text::Regular,sf::Text::Italic,8);
    s_names[vectorLength].setPosition(40,195+(vectorLength)*45 );;
    //
    s_leftButton.emplace_back("None",font,fontSize,player[vectorLength].color,player[vectorLength].color,sf::Text::Regular,sf::Text::Italic);
    s_leftButton[vectorLength].setPosition(200,195+(vectorLength)*45 );
    //
    s_rightButton.emplace_back("None",font,fontSize,player[vectorLength].color,player[vectorLength].color,sf::Text::Regular,sf::Text::Italic);
    s_rightButton[vectorLength].setPosition(300,195+(vectorLength)*45 );
    //
    s_status.emplace_back("W00T",font,fontSize);
    s_status[vectorLength].setColor(sf::Color::White);
    s_status[vectorLength].setPosition(670 , 240);
    //
    s_kick.emplace_back("Kick!",font,fontSize,player[vectorLength].color,player[vectorLength].color,sf::Text::Regular,sf::Text::Italic);
    s_kick[vectorLength].setPosition(400,195+(vectorLength)*45 );
    // Game
    // Name
    g_names.emplace_back(player[vectorLength].name,font,fontSize*0.75);
    g_names[vectorLength].setColor(player[vectorLength].color);
    g_names[vectorLength].setPosition(config.windowWidth-config.statusWidth+5,50+vectorLength*30);
    // Score
    g_scores.emplace_back("0",font,fontSize*0.75);
    g_scores[vectorLength].setColor(player[vectorLength].color);
    g_scores[vectorLength].setPosition(config.windowWidth-30,50+vectorLength*30);
    //
    vectorLength++;
}
//
void RendererObjects::setOptions(const Game &game){
    //
    // Points
    if(game.maxPoints==10){
        s_max10.setActive(true);
        s_max20.setActive(false);
        s_max40.setActive(false);
    }
    else if(game.maxPoints==20){
        s_max10.setActive(false);
        s_max20.setActive(true);
        s_max40.setActive(false);
    }
    else if(game.maxPoints==40){
        s_max10.setActive(false);
        s_max20.setActive(false);
        s_max40.setActive(true);
    }
    //
    // Powerup
    if(game.powerupEnabled){
        s_powerupOn.setActive(true);
        s_powerupOff.setActive(false);
    }
    else{
        s_powerupOn.setActive(false);
        s_powerupOff.setActive(true);
    }
    // Countdown
    if(game.countdownEnabled){
        s_countdownOn.setActive(true);
        s_countdownOff.setActive(false);
    }
    else{
        s_countdownOn.setActive(false);
        s_countdownOff.setActive(true);
    }
    // Multiple players
    if(game.multiplePlayersEnabled){
        s_multiplePlayersOn.setActive(true);
        s_multiplePlayersOff.setActive(false);
    }
    else{
        s_multiplePlayersOn.setActive(false);
        s_multiplePlayersOff.setActive(true);
    }
}
// Font Loader
sf::Font RendererObjects::loadFont(const std::string& name){
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
const char* RendererObjects::getKeyName( const sf::Keyboard::Key &key ) {
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
