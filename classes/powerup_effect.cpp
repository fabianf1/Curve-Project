// Contains functions and constructors for the powerup effect class
// Needed Header
#include "powerup_effect.h"
// Constructor
Powerup_Effect::Powerup_Effect(const Powerup::Type &Type,const float &Time,const int &ID){
    type=Type;
    time=Time;
    id=ID;
}
Powerup_Effect::Powerup_Effect(const int &Player,const Powerup::Type &Type,const Powerup::Impact &Impact,const float &Time,const int &ID){
    player=Player;
    type=Type;
    time=Time;
    id=ID;
    impact=Impact;
}
