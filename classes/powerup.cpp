// Header
#include "powerup.h"
// Constructor
Powerup::Powerup(const Powerup::Type &Type,const Powerup::Impact &Impact, const int &Chance, const float &Min, const int &Rand){
    type=Type;
    impact=Impact;
    spawn_chance=Chance;
    effect_min_disappear=Min;
    effect_rand_disappear=Rand;
}
// Overloads
sf::Packet& operator <<(sf::Packet& packet, const Powerup::Type &j){
    //int i=static_cast<int>(j);
    return packet << static_cast<int>(j);
}
sf::Packet& operator >>(sf::Packet& packet, Powerup::Type &j){
    int i;
    packet >> i;
    j=(Powerup::Type) i;
    return packet;
}
sf::Packet& operator <<(sf::Packet& packet, const Powerup::Impact &j){
    //int i=static_cast<int>(j);
    return packet << static_cast<int>(j);
}
sf::Packet& operator >>(sf::Packet& packet, Powerup::Impact &j){
    int i;
    packet >> i;
    j=(Powerup::Impact) i;
    return packet;
}

