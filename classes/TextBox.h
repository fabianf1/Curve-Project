// Defines the object storage classes
#ifndef CURVE_TEXTBOX
#define CURVE_TEXTBOX
// Standard Headers
#include <SFML/Graphics.hpp>
#include "ObjectButton.h"

// TextBox class
class TextBox : public ObjectButton<sf::Text>{
public:
    sf::Clock caretClock;
    sf::Text caret;
    virtual void process(Main &main);
    virtual void draw(sf::RenderWindow &window);
    TextBox(sf::Text &&i_drawable, std::function<bool(Main&,ObjectButton<sf::Text>&)> &&i_visible, std::function<void(Main&,ObjectButton<sf::Text>&)> &&i_action, std::function<void(Main&,ObjectButton<sf::Text>&)> &&i_update, unsigned int &i_id, const sf::Color &i_selectedColor,const unsigned int &i_selectedStyle);
};

#endif // CURVE_TEXTBOX
