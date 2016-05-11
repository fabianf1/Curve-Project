#include "Packet.h"

sf::Packet& operator <<(sf::Packet& packet, const Packet &j){
    //int i=static_cast<int>(j);
    return packet << static_cast<int>(j);
}
sf::Packet& operator >>(sf::Packet& packet, Packet &j){
    int i;
    packet >> i;
    j=(Packet)i;
    return packet;
}
