// Contains functions and constructors for the Pacer class
// Needed Header
#include "Pacer.h"
// Constructor
Pacer::Pacer(const unsigned int &i_minPace){
    setPace(i_minPace);
}
// Functions
void Pacer::setPace(const unsigned int &i_minPace){
    minRunTime=sf::seconds(1.f/i_minPace);
}
unsigned int Pacer::getPace(){
    return 1.f/minRunTime.asSeconds();
}
void Pacer::resetClock(){
    clock.restart();
}
void Pacer::pace(){
    sf::sleep(minRunTime-clock.getElapsedTime());
    clock.restart();
}
