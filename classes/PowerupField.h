// Contains PowerupField class declaration
#ifndef CURVE_POWERUP_FIELD
#define CURVE_POWERUP_FIELD
// Class Headers
#include "Powerup.h"
//
class PowerupField{
    public:
        // Vars
        int x,y;// Position of the powerup
        Powerup::Type type; // Type of powerup
        Powerup::Impact impact; // Self, enemy or all(0,1,2)
        float time1; // Time since appearance
        float time2; // Time before disappearing
        int id;
        int place; // Place in powerup vector
        // Constructors
        PowerupField(const int &X,const int &Y,const Powerup::Type &Type,const Powerup::Impact &Impact,const float &Time,int &ID);
        PowerupField(const int &X,const int &Y,const Powerup::Type &Type,const Powerup::Impact &Impact,const float &Time,int &ID,const int &Place);
};
#endif // CURVE_POWERUP_FIELD
