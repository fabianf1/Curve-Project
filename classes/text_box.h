// Contains Text_Box class declaration
#ifndef CURVE_TEXT_BOX
#define CURVE_TEXT_BOX
// SFML Headers
#include <SFML/Graphics.hpp>
#include <math.h>
// Class Headers
#include "text_button.h"
//#include "game.h"
//
class Text_Box{
    // Vars
    Text_Button text;
    sf::Text pointer;
    sf::Clock pointer_clock;
    bool typing;
    int pos_x,pos_y;
    unsigned int max_size;
    public:
        // Constructor
        Text_Box();
        Text_Box(const sf::String &str,const sf::Font &font,const int &fontsize, const sf::Color &n_col, const sf::Color &s_col,const int &n_sty, const int &s_sty,const unsigned int &m_size);
        // Functions
        void setPosition(const float &x, const float &y);
        //void setPointer(const float &x, const float &y);
        void setString(const sf::String &str);
        sf::String getString();
        void setColors(const sf::Color &n_col, const sf::Color &s_col);
        void setActive(const bool &active); // HERE
        void Switch();
        bool Check(sf::RenderWindow &window);
        void Event(const sf::Event &event);
        void Draw(sf::RenderWindow &window);

};
#endif // CURVE_TEXT_BOX

