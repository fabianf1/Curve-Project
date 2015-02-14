// Contains game class declaration
#ifndef CURVE_POWERUP_EFFECT
#define CURVE_POWERUP_EFFECT
// Class Headers
#include "Powerup.h"
//
class PowerupEffect{
    public:
        // Vars
        int player;
        Powerup::Type type;
        Powerup::Impact impact;
        float time;
        int id;
        // Constructor
        PowerupEffect(const Powerup::Type &Type,const float &Time,const int &ID); //  Game effects
        PowerupEffect(const int &Player,const Powerup::Type &Type,const Powerup::Impact &Impact,const float &Time,const int &ID); // Player effects
};
#endif // CURVE_POWERUP_EFFECT
