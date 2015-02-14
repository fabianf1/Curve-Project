// Contains functions and constructors for the TextBox class
// Needed Header
#include "TextBox.h"
// Constructor
TextBox::TextBox(){
    //
    pointer.setString("|");
    setActive(false);
    maxSize=10;
}
TextBox::TextBox(const sf::String &string, const sf::Font &font, const int &fontSize, const sf::Color &i_normalColor, const sf::Color &i_selectedColor, const int &i_normalStyle, const int &i_selectedStyle, const int &i_maxSize){
    text.setButton(string,font,fontSize,i_normalColor,i_selectedColor,i_normalStyle,i_selectedStyle);
    pointer.setFont(font);
    pointer.setCharacterSize(fontSize);
    pointer.setColor(i_normalColor);
    pointer.setStyle(i_normalStyle);
    pointer.setString("|");
    setActive(false);
    maxSize=i_maxSize;
}
// Functions
void TextBox::setPosition(const float &x, const float &y){
    text.setPosition(x,y);
    xPosition=x;
    yPosition=y;
    pointer.setPosition(xPosition+text.getLocalBounds().width*1.01,yPosition);
}
//
void TextBox::setString(const sf::String &string){
    text.setString(string);
}
//
sf::String TextBox::getString(){
    return text.getString();
}
//
void TextBox::setColors(const sf::Color &i_normalColor, const sf::Color &i_selectedColor){
    text.setColors(i_normalColor,i_selectedColor);
}
//
void TextBox::setActive(const bool &active){
    typing=active;
    text.setActive(active);
}
//
void TextBox::toggle(){
    typing=!typing;
    text.setActive(typing);
}
//
bool TextBox::check(sf::RenderWindow &window){
    return text.check(window);
}
//
void TextBox::event(const sf::Event &event){
    //
    if(event.type==sf::Event::TextEntered&&( (event.text.unicode>=65&&event.text.unicode<=122)|| (event.text.unicode>=48&&event.text.unicode<=57) ) && text.getString().getSize()<maxSize){
        // This part below is to only allow the first letter to be a capital letter.
        if(text.getString().getSize()>0&&event.text.unicode>=65&&event.text.unicode<=90){
            int ch=event.text.unicode+32;
            text.setString(text.getString()+static_cast<char>(ch));
        }
        else{
            text.setString(text.getString()+static_cast<char>(event.text.unicode));
        }
    }
    else if(event.type==sf::Event::KeyPressed&&event.key.code == sf::Keyboard::BackSpace&&text.getString().getSize()>0){
        sf::String temp=text.getString();
        temp.erase(temp.getSize()-1,1);
        text.setString(temp);
    }
    pointer.setPosition(xPosition+text.getLocalBounds().width+5,yPosition);
}
//
void TextBox::draw(sf::RenderWindow &window){
    window.draw(text);
    if(typing && fmod(pointerClock.getElapsedTime().asSeconds(),1.0) > 0.5 ){
        window.draw(pointer);
    }
}
