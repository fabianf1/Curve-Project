// Contains TextBox class declaration
#ifndef CURVE_TEXT_BOX
#define CURVE_TEXT_BOX
//Headers
#include <math.h>
#include <SFML/Graphics.hpp>
#include "TextButton.h"
//
class TextBox{
    // Vars
    TextButton text;
    sf::Text pointer;
    sf::Clock pointerClock;
    bool typing;
    int xPosition,yPosition;
    unsigned int maxSize;
    public:
        // Constructor
        TextBox();
        TextBox(const sf::String &string, const sf::Font &font, const int &fontSize, const sf::Color &i_normalColor, const sf::Color &i_selectedColor, const int &i_normalStyle, const int &i_selectedStyle, const int &i_maxSize);
        // Functions
        void setPosition(const float &x, const float &y);
        void setString(const sf::String &string);
        sf::String getString();
        void setColors(const sf::Color &i_normalColor, const sf::Color &i_selectedColor);
        void setActive(const bool &active);
        void toggle();
        bool check(sf::RenderWindow &window);
        void event(const sf::Event &event);
        void draw(sf::RenderWindow &window);

};
#endif // CURVE_TEXT_BOX

