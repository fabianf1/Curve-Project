// Contains functions and constructors for the powerup effect class
// Needed Header
#include "powerup_effect.h"
// Constructor
Powerup_Effect::Powerup_Effect(const int &Type,const float &Time,const int &ID){
    type=Type;
    time=Time;
    id=ID;
}
Powerup_Effect::Powerup_Effect(const int &Player,const int &Type,const int &Impact,const float &Time,const int &ID){
    player=Player;
    type=Type;
    time=Time;
    id=ID;
    impact=Impact;
}
