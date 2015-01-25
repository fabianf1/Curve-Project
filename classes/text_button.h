// Contains game class declaration
#ifndef CURVE_TEXT_BUTTON
#define CURVE_TEXT_BUTTON
// SFML Headers
#include <SFML/Graphics.hpp>
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
        void setColors(const sf::Color &color,const sf::Color &s_color); // Different from setColor, which handles the drawing color, this just set two variables to be used by selected;
        void setStyles(const int &style,const int &s_style); // Same goes Here
        void setButton(const sf::String &str,const sf::Font &font,const unsigned int &fontsize,const sf::Color &n_col,const sf::Color &s_col,const int n_sty,const int s_sty);
        void setActive(const bool &active);
        bool getActive();
        // Constructor
        Text_Button(){};
        Text_Button(const sf::String &str,const sf::Font &font,const unsigned int &fontsize,const sf::Color &n_col,const sf::Color &s_col,const int n_sty,const int s_sty);
};
#endif // CURVE_TEXT_BUTTON
