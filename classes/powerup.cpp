// Contains functions and constructors for the Powerup class
// Header
#include "powerup.h"
//
Powerup::Powerup(const int &X,const int &Y,const int &Type,const int &Impact,const float &Time,int &ID){
    x=X;
    y=Y;
    type=Type;
    impact=Impact;
    time1=0;
    time2=Time;
    id=ID;
}
