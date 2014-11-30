// Contains game class declaration
#ifndef CURVE_POWERUP_EFFECT
#define CURVE_POWERUP_EFFECT
// Needed Headers
#include <SFML/Graphics.hpp>
//#include <iostream>
//#include <vector>
//
class Powerup_Effect{
    public:
        // Vars
        int player;
        int type;
        int impact;
        float time;
        int id;
        // Constructor
        Powerup_Effect(const int &Type,const float &Time,const int &ID); //  Game effects
        Powerup_Effect(const int &Player,const int &Type,const int &Impact,const float &Time,const int &ID); // Player effects
};
#endif // CURVE_POWERUP_EFFECT
