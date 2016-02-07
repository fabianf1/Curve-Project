// Defines the object storage classes
#ifndef CURVE_OBJECTHEAD
#define CURVE_OBJECTHEAD
// Standard Headers
#include <SFML/Graphics.hpp>
#include "ObjectBase.h"
//
class ObjectHead : public ObjectBase{
public:
    bool checkVisible(Main &main);
    void draw(sf::RenderWindow &window); // Will need more info for darkness
    sf::CircleShape *circle;
    sf::RectangleShape *rectangle;
    bool *death;
    bool *rightAngle;
    unsigned int id;
    ObjectHead(sf::CircleShape *i_circle, sf::RectangleShape *i_rectangle, bool *i_death, bool *i_rightAngle, unsigned int &i_id);
};

#endif // CURVE_OBJECTHEAD
