#include "Color.h"

sf::Packet& operator << (sf::Packet& packet, const sf::Color &j){
    return packet << j.r << j.g << j.b << j.a;
}
sf::Packet& operator >> (sf::Packet& packet, sf::Color &j){
    sf::Uint8 r,g,b,a;
    packet >> r >> g >> b >> a;
    j=sf::Color(r,g,b,a);
    return packet;
}
