// Contains config class declaration
#ifndef CURVE_POWERUP
#define CURVE_POWERUP
// Needed Headers
#include "..\curve.h"

class Powerup{
    public:
        // Vars
        int x,y;// Position of the powerup
        int type; // Type of powerup
        int impact; // Self, enemy or all(0,1,2)
        float time1; // Time since appearance
        float time2; // Time before disappearing
        int id;
        // Functions

        // Constructors
        Powerup(const int &X,const int &Y,const int &Type,const int &Impact,const float &Time,int &ID);
};
#endif // CURVE_POWERUP
