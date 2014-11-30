// Contains game class declaration
#ifndef CURVE_PROMPT
#define CURVE_PROMPT
// Needed Headers
#include "../curve.h"
//#include <iostream>
//#include <vector>
//
class Prompt{
    // Vars
    sf::RectangleShape border;
    std::vector<sf::RectangleShape> field_border;
    std::vector<sf::Text> field_text;
    std::vector<sf::Text> field_title;
    Text_Button confirm;
    public:
        // Functions
        void setPrompt(int number,sf::String fields[],sf::Font &font,int fontsize,int x, int y); // color and style hardcoded;
        void Empty(); // To empty the fields
        // Constructor
};
#endif // CURVE_PROMPT

