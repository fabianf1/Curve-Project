// Contains functions and constructors for the game setup class
// Needed Header
#include "..\curve.h"

// Constructor
Game_Setup::Game_Setup(const Config &config){
    // Title
    title.setString("Game Setup");
    title.setFont(config.font);
    title.setCharacterSize(config.font_title_size);
    title.setColor(sf::Color::White);
    title.setStyle(sf::Text::Bold);
    title.setPosition( (config.window_width/2)-(title.getLocalBounds().width/2) ,10); // Top Middle
    // Name
    name.setString("Name");
    name.setFont(config.font);
    name.setCharacterSize(config.font_size);
    name.setColor(sf::Color::White);
    name.setStyle(sf::Text::Bold);
    name.setPosition(50 , 150);
    // Left
    left.setString("Left");
    left.setFont(config.font);
    left.setCharacterSize(config.font_size);
    left.setColor(sf::Color::White);
    left.setStyle(sf::Text::Bold);
    left.setPosition(200 , 150);
    // Right
    right.setString("Right");
    right.setFont(config.font);
    right.setCharacterSize(config.font_size);
    right.setColor(sf::Color::White);
    right.setStyle(sf::Text::Bold);
    right.setPosition(300 , 150);
    //
    for(int i=0;i<MAX_PLAYERS;i++){
        //
        names[i].setButton("Player " + int2string(i+1),config.font,config.font_size,config.linecolors[i],config.linecolors[i],sf::Text::Regular,sf::Text::Regular);
        names[i].setPosition( 50 , 185+35*i);
        //
        lbutton[i].setButton("",config.font,config.font_size,config.linecolors[i],config.linecolors[i],sf::Text::Regular,sf::Text::Regular);
        lbutton[i].setPosition(200 , 185+35*i);
        //
        rbutton[i].setButton("",config.font,config.font_size,config.linecolors[i],config.linecolors[i],sf::Text::Regular,sf::Text::Regular);
        rbutton[i].setPosition(300 , 185+35*i);
    }
    //
    options.setString("Options");
    options.setFont(config.font);
    options.setCharacterSize(config.font_size);
    options.setColor(sf::Color::White);
    options.setStyle(sf::Text::Bold);
    options.setPosition(750 , 150);
    //
    maxpoints.setString("Max Points:");
    maxpoints.setFont(config.font);
    maxpoints.setCharacterSize(config.font_size);
    maxpoints.setColor(sf::Color::White);
    //maxpoints.setStyle(sf::Text::Regular);
    maxpoints.setPosition(670 , 200);
    //
    max10.setButton("10",config.font,config.font_size,sf::Color::Red,sf::Color::Yellow,sf::Text::Regular,sf::Text::Italic);
    max10.setPosition(830,200);
    //
    max20.setButton("20",config.font,config.font_size,sf::Color::Red,sf::Color::Yellow,sf::Text::Regular,sf::Text::Italic);
    max20.setPosition(870,200);
    //
    max40.setButton("40",config.font,config.font_size,sf::Color::Red,sf::Color::Yellow,sf::Text::Regular,sf::Text::Italic);
    max40.setPosition(910,200);
    //
    powerups.setString("Powerups:");
    powerups.setFont(config.font);
    powerups.setCharacterSize(config.font_size);
    powerups.setColor(sf::Color::White);
    //powerups.setStyle((sf::Text::Regular);
    powerups.setPosition(670 , 240);
    //
    powerupon.setButton("On",config.font,config.font_size,sf::Color::Red,sf::Color::Yellow,sf::Text::Regular,sf::Text::Italic);
    powerupon.setPosition(830,240);
    //
    powerupoff.setButton("Off",config.font,config.font_size,sf::Color::Red,sf::Color::Yellow,sf::Text::Regular,sf::Text::Italic);
    powerupoff.setPosition(880,240);
    //
    start.setButton("Start Game",config.font,config.font_size,sf::Color::Red,sf::Color::Yellow,sf::Text::Regular,sf::Text::Italic);
    start.setPosition(config.window_width/2-start.getLocalBounds().width/2,config.window_height-100);
    //
    quit.setButton("Quit",config.font,config.font_size,sf::Color::Red,sf::Color::Yellow,sf::Text::Regular,sf::Text::Italic);
    quit.setPosition(config.window_width/2-quit.getLocalBounds().width/2,config.window_height-50);

}
