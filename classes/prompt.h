// Contains Prompt class declaration
#ifndef CURVE_PROMPT
#define CURVE_PROMPT
// SFML Headers
#include <SFML/Graphics.hpp>
// Class Headers
#include "TextButton.h"
//
class Prompt{
    // Vars
    bool enabled;
    int xPosition,yPosition;
    sf::RectangleShape border;
    sf::Text text;
    sf::Text title;
    TextButton confirm;
    public:
        // Functions
        void setPrompt(const sf::String &field,const sf::Font &font,const unsigned int &fontSize,const int &x,const int &y); // color and style hardcoded;
        void empty(); // To empty the fields
        void draw(sf::RenderWindow &window);
        void event(sf::Event &event);
        void toggle();
        bool isEnabled();
        bool check(sf::RenderWindow &window);
        void setString(const sf::String &string);
        sf::String getString();
        // Constructor
        Prompt(): enabled(false){};
};
#endif // CURVE_PROMPT

