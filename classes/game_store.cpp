// Contains functions and constructors for the game stor class
// Needed Header
#include "..\curve.h"

// Constructor
Game_Store::Game_Store(const Config &config){
    // Left and right whole walls, top and bottom not
    // Left
    leftwall.setFillColor(config.wallcolor);
    leftwall.setSize(sf::Vector2f(config.wallwidth, config.window_height));
    // Top
    topwall.setSize(sf::Vector2f(config.window_width-config.statuswidth-2*config.wallwidth,config.wallwidth));
    topwall.setPosition(config.wallwidth,0);
    // Right
    rightwall.setSize(sf::Vector2f(config.wallwidth, config.window_height));
    rightwall.setPosition(config.window_width-config.wallwidth-config.statuswidth,0);
    // Bottom
    bottomwall.setSize(sf::Vector2f(config.window_width-config.statuswidth-2*config.wallwidth,config.wallwidth));
    bottomwall.setPosition(config.wallwidth,config.window_height-config.wallwidth);
    //
    int title_size=25;
    int normal_size=15;
    // Scores title
    scores.setFont(config.font);
    scores.setString("Scores");
    scores.setCharacterSize(title_size);
    scores.setPosition( config.window_width-config.statuswidth/2-scores.getLocalBounds().width/2,10);
    scores.setColor(sf::Color::White);
    scores.setStyle(sf::Text::Bold);
    // scores
    for(int i=0;i<MAX_PLAYERS;i++){
        // Name
        names[i].setCharacterSize(normal_size);
        names[i].setFont(config.font);
        names[i].setColor(config.linecolors[i]);
        names[i].setString("Player " + int2string(i+1));
        names[i].setPosition(config.window_width-config.statuswidth+5,50+30*i);
        // Score
        score[i].setCharacterSize(normal_size);
        score[i].setFont(config.font);
        score[i].setColor(config.linecolors[i]);
        score[i].setPosition(config.window_width-30,50+30*i);
    }
    // Round
    round[0].setCharacterSize(normal_size);
    round[0].setFont(config.font);
    round[0].setColor(sf::Color::Yellow);
    round[0].setString("Round:");
    round[0].setPosition(config.window_width-config.statuswidth+5,config.window_height/2);
    round[1].setCharacterSize(normal_size);
    round[1].setFont(config.font);
    round[1].setColor(sf::Color::Yellow);
    round[1].setPosition(config.window_width-config.statuswidth+100,config.window_height/2);
    // Frame
    frame[0].setCharacterSize(normal_size);
    frame[0].setFont(config.font);
    frame[0].setColor(sf::Color::Yellow);
    frame[0].setString("Frame:");
    frame[0].setPosition(config.window_width-config.statuswidth+5,config.window_height/2+30);
    frame[1].setCharacterSize(normal_size);
    frame[1].setFont(config.font);
    frame[1].setColor(sf::Color::Yellow);
    frame[1].setPosition(config.window_width-config.statuswidth+100,config.window_height/2+30);
    // FPS
    fps[0].setCharacterSize(normal_size);
    fps[0].setFont(config.font);
    fps[0].setColor(sf::Color::Yellow);
    fps[0].setString("FPS:");
    fps[0].setPosition(config.window_width-config.statuswidth+5,config.window_height/2+60);
    fps[1].setCharacterSize(normal_size);
    fps[1].setFont(config.font);
    fps[1].setColor(sf::Color::Yellow);
    fps[1].setPosition(config.window_width-config.statuswidth+100,config.window_height/2+60);
    // Packet
    packet[0].setCharacterSize(normal_size);
    packet[0].setFont(config.font);
    packet[0].setColor(sf::Color::Yellow);
    packet[0].setString("PPS:");
    packet[0].setPosition(config.window_width-config.statuswidth+5,config.window_height/2+90);
    packet[1].setCharacterSize(normal_size);
    packet[1].setFont(config.font);
    packet[1].setColor(sf::Color::Yellow);
    packet[1].setPosition(config.window_width-config.statuswidth+100,config.window_height/2+90);
    // Quit Button
    quit.setButton("Quit",config.font,normal_size,sf::Color::Red,sf::Color::Yellow,sf::Text::Bold,sf::Text::Bold);
    quit.setPosition(config.window_width-config.statuswidth/2-quit.getLocalBounds().width/2,config.window_height-50);
    // End round message
    end_round_message.setFont(config.font);
    end_round_message.setCharacterSize(50);
    end_round_message.setStyle(sf::Text::Bold);
    // Countdown
    countdown.setFont(config.font);
    countdown.setCharacterSize(50);
    countdown.setStyle(sf::Text::Bold);
}
