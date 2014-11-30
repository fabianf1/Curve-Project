// Contains functions and constructors for the main_menu class
// Needed Header
#include "..\curve.h"

// Constructor
Main_Menu::Main_Menu(const Config &config){
    // Title
    title.setString(config.title);
    title.setFont(config.font);
    title.setCharacterSize(config.font_title_size);
    title.setColor(sf::Color::White);
    title.setStyle(sf::Text::Bold);
    title.setPosition( (config.window_width/2)-(title.getLocalBounds().width/2) ,10);
    // Start
    start_game.setButton("Start game",config.font,config.font_size,sf::Color::Red,sf::Color::Yellow,sf::Text::Regular,sf::Text::Regular);
    start_game.setPosition( 50 , config.window_height/2);
    // Server
    server.setButton("Start server",config.font,config.font_size,sf::Color::Red,sf::Color::Yellow,sf::Text::Regular,sf::Text::Regular);
    server.setPosition( 50 , config.window_height/2+40);
    // Client
    client.setButton("Connect to server",config.font,config.font_size,sf::Color::Red,sf::Color::Yellow,sf::Text::Regular,sf::Text::Regular);
    client.setPosition( 50 , config.window_height/2+80); // Left side
    // Quit
    quit.setButton("Quit",config.font,config.font_size,sf::Color::Red,sf::Color::Yellow,sf::Text::Regular,sf::Text::Regular);
    quit.setPosition( 50 , config.window_height/2+120);
    //
    outline.setSize(sf::Vector2f(200, 125));
    outline.setFillColor(config.window_backgroundcolor);
    outline.setOutlineColor(sf::Color::Red);
    outline.setOutlineThickness(2);
    outline.setPosition(config.window_width/2 -outline.getLocalBounds().width/2,config.window_height/2-outline.getLocalBounds().height/2);
    //
    connect_title.setString("Enter IP:");
    connect_title.setFont(config.font);
    connect_title.setCharacterSize(config.font_size);
    connect_title.setColor(sf::Color::White);
    connect_title.setStyle(sf::Text::Regular);
    connect_title.setPosition( (config.window_width/2)-(connect_title.getLocalBounds().width/2) ,config.window_height/2-outline.getLocalBounds().height/2 + 5);
    //
    ip.setString("");
    ip.setFont(config.font);
    ip.setCharacterSize(config.font_size);
    ip.setColor(sf::Color::Yellow);
    ip.setStyle(sf::Text::Regular);
    ip.setPosition( (config.window_width/2)-(ip.getLocalBounds().width/2) ,config.window_height/2-outline.getLocalBounds().height/2 + 40);
    //
    connect.setButton("Connect",config.font,config.font_size,sf::Color::Red,sf::Color::Yellow,sf::Text::Regular,sf::Text::Italic);
    connect.setPosition( (config.window_width/2)-(connect.getLocalBounds().width/2) ,config.window_height/2-outline.getLocalBounds().height/2 + 70);
}

