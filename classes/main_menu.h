// Contains Main_Menu class declaration
#ifndef CURVE_MAIN_MENU
#define CURVE_MAIN_MENU
// SFML Headers
#include <SFML/Graphics.hpp>
//
//#include <iostream>
//#include <vector>
// Class headers
#include "text_button.h"
//
class Main_Menu{
    public:
        // Vars
        sf::Text title;
        Text_Button start_game;
        Text_Button server;
        Text_Button client;
        Text_Button quit;
        // define a 120x50 rectangle
        sf::RectangleShape outline;
        sf::Text connect_title;
        sf::Text ip;
        Text_Button connect;
        // Constructor
        Main_Menu(const Config &config);
};
#endif // CURVE_MAIN_MENU

