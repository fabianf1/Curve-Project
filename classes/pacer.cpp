// Contains functions and constructors for the Pacer class
// Needed Header
#include "Pacer.h"
// Constructor
Pacer::Pacer(const unsigned int &i_minRunTime){
    setPace(i_minRunTime);
}
// Functions
void Pacer::setPace(const unsigned int &i_minRunTime){
    minRunTime=i_minRunTime;
}
unsigned int Pacer::getPace(){
    return minRunTime;
}
void Pacer::resetClock(){
    clock.restart();
}
void Pacer::pace(){
    int wait=minRunTime-clock.restart().asMilliseconds();
    if(wait>0){
        sf::sleep(sf::milliseconds(wait));
    }
}
