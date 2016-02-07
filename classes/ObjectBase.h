// Defines the object storage classes
#ifndef CURVE_OBJECTBASE
#define CURVE_OBJECTBASE
// Standard Headers
#include <SFML/Graphics.hpp>
#include "functional"
// Forward declaration
class Main;

// Base class. Defines the functions and variables that need to be accessed from the outside
class ObjectBase{
public:
    virtual bool checkVisible(Main &main) = 0;
    virtual void process(Main &main){
        return;
    }
    virtual void draw(sf::RenderWindow &window) = 0;
};

#endif // CURVE_OBJECTBASE
