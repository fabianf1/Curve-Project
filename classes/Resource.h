// Contains Resource loading things
#ifndef CURVE_RESOURCE
#define CURVE_RESOURCE
// Standard Headers
#include <string>
#include <SFML/Graphics.hpp>
//
class Resource{
    public:
        static sf::Image loadImage(const std::string& name);
        static sf::Font loadFont(const std::string& name);
};
//
#endif // CURVE_RESOURCE
