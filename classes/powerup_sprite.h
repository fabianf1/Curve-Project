// Contains powerup sprite class declaration
#ifndef CURVE_POWERUP_SPRITE
#define CURVE_POWERUP_SPRITE
// Needed Headers
#include "..\curve.h"
//#include <iostream>
//#include <vector>
//
class Powerup_Sprite{
    sf::Texture texture;// The texture that has all powerups
public:
    sf::Sprite fastgreen,fastred; // Sprites for the fastpowerup
    sf::Sprite slowgreen,slowred;
    sf::Sprite smallgreen,smallred;
    sf::Sprite biggreen,bigred;
    sf::Sprite anglegreen,anglered;
    sf::Sprite clearblue;
    sf::Sprite invisiblegreen;
    sf::Sprite wallsawayblue;
    sf::Sprite morepowerblue;
    sf::Sprite invertedred;
    //sf::Sprite allpowerups;
    // Constructor
    Powerup_Sprite(const Config &config);
};
#endif // CURVE_POWERUP_SPRITE
