// Contains functions and constructors for the TextButton class
// Needed Header
#include "TextButton.h"
// Constructor
TextButton::TextButton(const sf::String &string,const sf::Font &font,const unsigned int &fontSize,const sf::Color &i_normalColor,const sf::Color &i_selectedColor,const int &i_normalStyle,const int &i_selectedStyle){
    setButton(string,font,fontSize,i_normalColor,i_selectedColor,i_normalStyle,i_selectedStyle);
}
// Functions
bool TextButton::check(sf::RenderWindow &window){
    // check selected
    if(getGlobalBounds().contains(sf::Mouse::getPosition(window).x,sf::Mouse::getPosition(window).y)){
        setColor(selectedColor);
        if(!activated){
            setStyle(selectedStyle);
        }
        else{
            setStyle(selectedStyle|activatedStyle);
        }
        if(window.hasFocus()&&sf::Mouse::isButtonPressed(sf::Mouse::Left)&&lastClick.getElapsedTime().asSeconds()>0.2){
            lastClick.restart();
            return true;
        }
    }
    else{
        setColor(normalColor);
        if(!activated){
            setStyle(normalStyle);
        }
        else{
            setStyle(normalStyle|activatedStyle);
        }
    }
    return false;
}
//
void TextButton::setColors(const sf::Color &i_normalColor,const sf::Color &i_selectedColor){
    normalColor=i_normalColor;
    selectedColor=i_selectedColor;
}
//
void TextButton::setStyles(const int &i_normalStyle,const int &i_selectedStyle){
    normalStyle=i_normalStyle;
    selectedStyle=i_selectedStyle;
}
//
void TextButton::setButton(const sf::String &string,const sf::Font &font,const unsigned int &fontSize,const sf::Color &i_normalColor,const sf::Color &i_selectedColor,const int &i_normalStyle,const int &i_selectedStyle){
    setString(string);
    setFont(font);
    setCharacterSize(fontSize);
    setColors(i_normalColor,i_selectedColor);
    setStyles(i_normalStyle,i_selectedStyle);
    activatedStyle=sf::Text::Bold|sf::Text::Underlined ;
    activated=false;
}
//
 void TextButton::setActive(const bool &active){
     activated=active;
 }
 //
 bool TextButton::isActive(){
    return activated;
 }
