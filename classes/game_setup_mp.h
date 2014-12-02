// Contains Game Setup class declaration
#ifndef CURVE_GAME_SETUP_MP
#define CURVE_GAME_SETUP_MP
// SFML Headers
#include "SFML/Graphics.hpp"
// Class Headers
#include "text_button.h"
#include "config.h"
// Temporary
#include "..\defines.h"
//
class Game_Setup_MP{
    public:
        // Vars
        sf::Text title;
        sf::Text name;
        sf::Text name_pointer;
        Text_Button name_field;
        sf::Text left;
        Text_Button keyL;
        sf::Text right;
        Text_Button keyR;
        sf::Text color;
        // Color chooser not ready :D

        //
        sf::Text other_names;
        sf::Text names[MAX_PLAYERS-1];
        sf::Text ready[MAX_PLAYERS-1]; // To show who is ready and server
        //
        sf::Text status;
        //
        sf::Text options;
        sf::Text maxpoints;
        Text_Button max10,max20,max40;
        sf::Text powerups;
        Text_Button powerupon,powerupoff;
        //
        Text_Button start; // Servers as start and ready button
        Text_Button quit;
        // Constructor
        Game_Setup_MP(const Config &config);
};
#endif // CURVE_GAME_SETUP_MP


