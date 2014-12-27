// Contains game class declaration
#ifndef CURVE_TEXT_BUTTON
#define CURVE_TEXT_BUTTON
// SFML Headers
#include <SFML/Graphics.hpp>
// Class Headers
#include "game.h"
//
class Text_Button: public sf::Text{
    // Vars
    sf::Color normal_color,selected_color;
    int normal_style,selected_style, activated_style;
    sf::Clock last_click;
    bool activated;
    public:
        // Functions
        bool Check(sf::RenderWindow &window);// Handless Click and selected, returns true only if clicked
        void setColors(sf::Color color, sf::Color s_color); // Different from setColor, which handles the drawing color, this just set two variables to be used by selected;
        void setStyles(int style, int s_style); // Same goes Here
        void setButton(sf::String str,const sf::Font &font,int fontsize,sf::Color col,sf::Color selected_col,int sty,int selected_sty);
        void setActive(const bool &active);
        // Constructor
        Text_Button(){};
        Text_Button(sf::String str,const sf::Font &font,int fontsize,sf::Color col,sf::Color selected_col,int sty,int selected_sty);
};
#endif // CURVE_TEXT_BUTTON
