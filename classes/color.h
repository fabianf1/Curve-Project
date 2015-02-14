#ifndef CURVE_COLOR
#define CURVE_COLOR

#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>

// Overload Packer class for color
sf::Packet& operator << (sf::Packet& packet, const sf::Color &j);
sf::Packet& operator >> (sf::Packet& packet, sf::Color &j);

#endif // CURVE_COLOR
