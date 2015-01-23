// Contains Prompt class declaration
#ifndef CURVE_PROMPT
#define CURVE_PROMPT
// Standard Headers
//#include <String
// SFML Headers
#include <SFML/Graphics.hpp>
// Class Headers
#include "text_button.h"
//
class Prompt{
    // Vars
    bool enabled;
    int x,y;
    sf::RectangleShape border;
    sf::Text text;
    sf::Text title;
    Text_Button confirm;
    public:
        // Functions
        void setPrompt(const sf::String &field,const sf::Font &font,const unsigned int &fontsize,const int &X,const int &Y); // color and style hardcoded;
        void Empty(); // To empty the fields
        void Draw(sf::RenderWindow &window);
        void Event(sf::Event &event);
        void Switch();
        bool Check(sf::RenderWindow &window);
        void setString(const sf::String &string);
        sf::String getString();
        // Constructor
        Prompt(): enabled(false){};
};
#endif // CURVE_PROMPT

