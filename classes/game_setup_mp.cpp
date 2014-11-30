// Contains functions and constructors for the game setup class
// Needed Header
#include "..\curve.h"

// Constructor
Game_Setup_MP::Game_Setup_MP(const Config &config){
    // Title
    title.setString("Game Setup");
    title.setFont(config.font);
    title.setCharacterSize(config.font_title_size);
    title.setColor(sf::Color::White);
    title.setStyle(sf::Text::Bold);
    title.setPosition( (config.window_width/2)-(title.getLocalBounds().width/2) ,10); // Top Middle
    // Name
    name.setString("Name:");
    name.setFont(config.font);
    name.setCharacterSize(config.font_size);
    name.setColor(sf::Color::White);
    name.setStyle(sf::Text::Bold);
    name.setPosition(50 , 150);
    name_field.setButton("Player",config.font,config.font_size,sf::Color::Red,sf::Color::Yellow,sf::Text::Regular,sf::Text::Italic);
    name_field.setPosition(200 , 150);
    // Name Pointer
    name_pointer.setString("|");
    name_pointer.setFont(config.font);
    name_pointer.setCharacterSize(config.font_size);
    name_pointer.setColor(sf::Color::White);
    name_pointer.setStyle(sf::Text::Bold);
    name_pointer.setPosition(50 , 150);
    // Left
    left.setString("Left:");
    left.setFont(config.font);
    left.setCharacterSize(config.font_size);
    left.setColor(sf::Color::White);
    left.setStyle(sf::Text::Bold);
    left.setPosition(50 , 185);
    keyL.setButton("",config.font,config.font_size,sf::Color::Red,sf::Color::Yellow,sf::Text::Regular,sf::Text::Italic);
    keyL.setPosition(200 , 185);
    // Right
    right.setString("Right:");
    right.setFont(config.font);
    right.setCharacterSize(config.font_size);
    right.setColor(sf::Color::White);
    right.setStyle(sf::Text::Bold);
    right.setPosition(50 , 220);
    keyR.setButton("",config.font,config.font_size,sf::Color::Red,sf::Color::Yellow,sf::Text::Regular,sf::Text::Italic);
    keyR.setPosition(200 , 220);
    // Color Chooser
    color.setString("Color:");
    color.setFont(config.font);
    color.setCharacterSize(config.font_size);
    color.setColor(sf::Color::White);
    color.setStyle(sf::Text::Bold);
    color.setPosition(50 , 255);
    // Other player
    other_names.setString("Other Players:");
    other_names.setFont(config.font);
    other_names.setCharacterSize(config.font_size);
    other_names.setColor(sf::Color::White);
    other_names.setStyle(sf::Text::Bold);
    other_names.setPosition(50 , 300);
    //
    for(int i=0;i<MAX_PLAYERS-1;i++){
        names[i].setString("");
        names[i].setFont(config.font);
        names[i].setCharacterSize(config.font_size);
        names[i].setColor(sf::Color::White);
        names[i].setStyle(sf::Text::Bold);
        names[i].setPosition(50 , 335+35*i);
        ready[i].setString("");
        ready[i].setFont(config.font);
        ready[i].setCharacterSize(config.font_size);
        ready[i].setColor(sf::Color::White);
        ready[i].setStyle(sf::Text::Bold);
        ready[i].setPosition(230 , 335+35*i);
    }
    //
    start.setButton("Start Game",config.font,config.font_size,sf::Color::Red,sf::Color::Yellow,sf::Text::Regular,sf::Text::Italic);
    start.setPosition(config.window_width/2-start.getLocalBounds().width/2,config.window_height-100);
    //
    quit.setButton("Quit",config.font,config.font_size,sf::Color::Red,sf::Color::Yellow,sf::Text::Regular,sf::Text::Italic);
    quit.setPosition(config.window_width/2-quit.getLocalBounds().width/2,config.window_height-50);
    //
    // Other player
    //status.setString("");
    status.setFont(config.font);
    status.setCharacterSize(15);
    status.setColor(sf::Color::White);
    status.setStyle(sf::Text::Regular);
    status.setPosition(10 , config.window_height-30);
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

}
