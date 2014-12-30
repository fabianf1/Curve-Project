// Contains functions and constructors for the Pacer class
// Needed Header
#include "pacer.h"
// Constructor
Pacer::Pacer(const unsigned int &Pace){
    setPace(Pace);
}
// Functions
void Pacer::setPace(const unsigned int &Pace){
    pace=Pace;
}
unsigned int Pacer::getPace(){
    return pace;
}
void Pacer::resetClock(){
    clock.restart();
}
void Pacer::Pace(){
    int wait=pace-clock.restart().asMilliseconds();
    if(wait>0){
        sf::sleep(sf::milliseconds(wait));
    }
}
