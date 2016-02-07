// Contains functions and constructors for Objects
// Needed Header
#include "ObjectHead.h"
#include "main.h"
// Head
bool ObjectHead::checkVisible(Main &main){
    if(main.player.size()>id){
        return true;
    }
    return false;
}
//
void ObjectHead::draw(sf::RenderWindow &window){
    //if(!*death){ //Death&Darkness!!
        if(*rightAngle){
            window.draw(*rectangle);
        }
        else{
            window.draw(*circle);
        }
    //}
}
//
ObjectHead::ObjectHead(sf::CircleShape *i_circle, sf::RectangleShape *i_rectangle, bool *i_death, bool *i_rightAngle, unsigned int &i_id){
    circle=i_circle;
    rectangle=i_rectangle;
    id=i_id;
    death=i_death;
    rightAngle=i_rightAngle;
}
//
