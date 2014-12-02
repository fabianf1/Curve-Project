// Contains Game Setup class declaration
#ifndef CURVE_GAME_SETUP
#define CURVE_GAME_SETUP
// SFML Headers
#include "SFML/Graphics.hpp"
// Class Headers
#include "text_button.h"
#include "config.h"
// Temporary
#include "../defines.h"
//
class Game_Setup{
    public:
        // Vars
        sf::Text title;
        sf::Text name;
        sf::Text left;
        sf::Text right;
        //
        Text_Button names[MAX_PLAYERS];
        Text_Button lbutton[MAX_PLAYERS];
        Text_Button rbutton[MAX_PLAYERS];
        //
        sf::Text options;
        sf::Text maxpoints;
        Text_Button max10,max20,max40;
        sf::Text powerups;
        Text_Button powerupon,powerupoff;
        //
        Text_Button start;
        Text_Button quit;
        // Constructor
        Game_Setup(const Config &config);
};
#endif // CURVE_GAME_SETUP


