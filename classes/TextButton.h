// Contains game class declaration
#ifndef CURVE_TEXTBUTTON
#define CURVE_TEXTBUTTON
// Headers
#include <SFML/Graphics.hpp>
//
class TextButton: public sf::Text{
    // Vars
    sf::Color normalColor,selectedColor;
    int normalStyle,selectedStyle, activatedStyle;
    sf::Clock lastClick;
    bool activated;
    public:
        // Functions
        bool check(sf::RenderWindow &window);// Handless Click and selected, returns true only if clicked
        void setColors(const sf::Color &i_normalColor,const sf::Color &i_selectedColor); // Different from setColor, which handles the drawing color, this just set two variables to be used by selected;
        void setStyles(const int &i_normalStyle,const int &i_selectedStyle); // Same goes Here
        void setButton(const sf::String &string,const sf::Font &font,const unsigned int &fontSize,const sf::Color &i_normalColor, const sf::Color &i_selectedColor, const int &i_normalStyle, const int &i_selectedStyle);
        void setActive(const bool &active);
        bool isActive();
        // Constructor
        TextButton(){};
        TextButton(const sf::String &string, const sf::Font &font, const unsigned int &fontSize, const sf::Color &i_normalColor, const sf::Color &i_selectedColor, const int &i_normalStyle, const int &i_selectedStyle);
};
#endif // CURVE_TEXTBUTTON
