// Contains functions and constructors for the Text_Button class
// Needed Header
#include "..\curve.h"
// Functions
bool Text_Button::Check(Game &game,sf::RenderWindow &window){
    // Check selected
    if(getGlobalBounds().contains(sf::Mouse::getPosition(window).x,sf::Mouse::getPosition(window).y)){
        setColor(selected_color);
        setStyle(selected_style);
        if(sf::Mouse::isButtonPressed(sf::Mouse::Left)&&game.lastclick+15<game.frame){
            game.lastclick=game.frame;
            return true;
        }
        return false;
    }
    else{
        setColor(normal_color);
        setStyle(normal_style);
    }
    return false;
}
//
void Text_Button::setColors(sf::Color color, sf::Color s_color){
    normal_color=color;
    selected_color=s_color;
}
void Text_Button::setStyles(int style, int s_style){
    normal_style=style;
    selected_style=s_style;
}
void Text_Button::setButton(sf::String str,const sf::Font &font,int fontsize,sf::Color col,sf::Color selected_col,int sty,int selected_sty){
    setString(str);
    setFont(font);
    setCharacterSize(fontsize);
    setColors(col,selected_col);
    setStyles(sty,selected_sty);
}
