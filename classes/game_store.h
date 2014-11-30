// Contains Game Store class declaration
#ifndef CURVE_GAME_STORE
#define CURVE_GAME_STORE
// Needed Headers
#include "..\curve.h"
//#include <iostream>
//#include <vector>
//
class Game_Store{
    public:
        // Vars
        sf::RectangleShape leftwall,topwall,rightwall,bottomwall;
        sf::Text scores;
        sf::Text names[MAX_PLAYERS];
        sf::Text score[MAX_PLAYERS];
        sf::Text round[2];
        sf::Text frame[2];
        sf::Text fps[2];
        sf::Text packet[2];
        Text_Button quit;
        sf::Text end_round_message;
        sf::Text countdown;
        // Constructor
        Game_Store(const Config &config);
};
#endif // CURVE_GAME_STORE
