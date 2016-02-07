// Defines the object storage classes
#ifndef CURVE_OBJECTBUTTON
#define CURVE_OBJECTBUTTON
// Standard Headers
#include <SFML/Graphics.hpp>
#include "ObjectBase.h"

// Button template class
template <class Drawable>
class ObjectButton : public ObjectBase{
public: // Should move
    Drawable drawable;
    std::function<bool(Main&,ObjectButton<Drawable>&)> visible; // Change this to true function
    std::function<void(Main&,ObjectButton<Drawable>&)> action;
    std::function<void(Main&,ObjectButton<Drawable>&)> update;
    sf::Clock lastClick;
    unsigned int normalStyle;
    unsigned int selectedStyle;
    unsigned int activatedStyle;
    sf::Color normalColor;
    sf::Color selectedColor;
    bool activated;
    unsigned int id;
    // Draw function
    virtual void draw(sf::RenderWindow &window);
    virtual bool checkVisible(Main &main);
    virtual void process(Main &main);
    //virtual void update(Main &main);
    bool check(sf::RenderWindow &window);
    // Constructor; Copying is required!
    ObjectButton(Drawable &&i_drawable, std::function<bool(Main&,ObjectButton<Drawable>&)> &&i_visible, std::function<void(Main&,ObjectButton<Drawable>&)> &&i_action,const sf::Color &i_selectedColor,const unsigned int &i_selectedStyle);
    ObjectButton(Drawable &&i_drawable, std::function<bool(Main&,ObjectButton<Drawable>&)> &&i_visible, std::function<void(Main&,ObjectButton<Drawable>&)> &&i_action, std::function<void(Main&,ObjectButton<Drawable>&)> &&i_update, unsigned int &i_id, const sf::Color &i_selectedColor,const unsigned int &i_selectedStyle);
};

#endif // CURVE_OBJECTBUTTON
