// Contains functions and constructors for the Powerup_Field class
// Header
#include "powerup_field.h"
//
Powerup_Field::Powerup_Field(const int &X,const int &Y,const Powerup::Type &Type,const Powerup::Impact &Impact,const float &Time,int &ID){
    x=X;
    y=Y;
    type=Type;
    impact=Impact;
    time1=0;
    time2=Time;
    id=ID;
}
//
Powerup_Field::Powerup_Field(const int &X,const int &Y,const Powerup::Type &Type,const Powerup::Impact &Impact,const float &Time,int &ID,const int &Place){
    x=X;
    y=Y;
    type=Type;
    impact=Impact;
    time1=0;
    time2=Time;
    id=ID;
    place=Place;
}
