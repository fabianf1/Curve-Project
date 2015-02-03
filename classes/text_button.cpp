// Contains functions and constructors for the Text_Button class
// Needed Header
#include "text_button.h"
// Constructor
Text_Button::Text_Button(const sf::String &str,const sf::Font &font,const unsigned int &fontsize,const sf::Color &n_col,const sf::Color &s_col,const int n_sty,const int s_sty){
    setButton(str,font,fontsize,n_col,s_col,n_sty,s_sty);
}
// Functions
bool Text_Button::Check(sf::RenderWindow &window){
    // Check selected
    if(getGlobalBounds().contains(sf::Mouse::getPosition(window).x,sf::Mouse::getPosition(window).y)){
        setColor(selected_color);
        if(!activated){
            setStyle(selected_style);
        }
        else{
            setStyle(selected_style|activated_style);
        }
        if(window.hasFocus()&&sf::Mouse::isButtonPressed(sf::Mouse::Left)&&last_click.getElapsedTime().asSeconds()>0.2){
            last_click.restart();
            return true;
        }
        return false;
    }
    else{
        setColor(normal_color);
        if(!activated){
            setStyle(normal_style);
        }
        else{
            setStyle(normal_style|activated_style);
        }
    }
    return false;
}
//
void Text_Button::setColors(const sf::Color &color,const sf::Color &s_color){
    normal_color=color;
    selected_color=s_color;
}
//
void Text_Button::setStyles(const int &style,const int &s_style){
    normal_style=style;
    selected_style=s_style;
}
//
void Text_Button::setButton(const sf::String &str,const sf::Font &font,const unsigned int &fontsize,const sf::Color &n_col,const sf::Color &s_col,const int n_sty,const int s_sty){
    setString(str);
    setFont(font);
    setCharacterSize(fontsize);
    setColors(n_col,s_col);
    setStyles(n_sty,s_sty);
    activated_style=sf::Text::Bold|sf::Text::Underlined ;
    activated=false;
}
//
 void Text_Button::setActive(const bool &active){
     activated=active;
 }
 //
 bool Text_Button::getActive(){
    return activated;
 }
