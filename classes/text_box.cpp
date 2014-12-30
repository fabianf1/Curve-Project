// Contains functions and constructors for the Text_Box class
// Needed Header
#include "text_box.h"
// Constructor
Text_Box::Text_Box(){
    //
    pointer.setString("|");
    setActive(false);
    max_size=10;
}
Text_Box::Text_Box(const sf::String &str,const sf::Font &font,const int &fontsize, const sf::Color &n_col, const sf::Color &s_col,const int &n_sty, const int &s_sty,const unsigned int &m_size){
    text.setButton(str,font,fontsize,n_col,s_col,n_sty,s_sty);
    pointer.setFont(font);
    pointer.setCharacterSize(fontsize);
    pointer.setColor(n_col);
    pointer.setStyle(n_sty);
    pointer.setString("|");
    setActive(false);
    max_size=m_size;
}
// Functions
void Text_Box::setPosition(const float &x, const float &y){
    text.setPosition(x,y);
    pos_x=x;
    pos_y=y;
    pointer.setPosition(pos_x+text.getLocalBounds().width*1.01,pos_y);
}
//
/*void Text_Box::setPointer(const float &x, const float &y){
    pointer.setPosition(x+text.getLocalBounds().width,y+text.getLocalBounds().height/2);
}*/
//
void Text_Box::setString(const sf::String &str){
    text.setString(str);
}
//
sf::String Text_Box::getString(){
    return text.getString();
}
//
void Text_Box::setColors(const sf::Color &n_col, const sf::Color &s_col){
    text.setColors(n_col,s_col);
}
//
void Text_Box::setActive(const bool &active){
    typing=active;
    text.setActive(active);
}
//
void Text_Box::Switch(){
    typing=!typing;
    text.setActive(typing);
}
//
bool Text_Box::Check(sf::RenderWindow &window){
    return text.Check(window);
}
//
void Text_Box::Event(const sf::Event &event){
    //
    if(event.type==sf::Event::TextEntered&&( (event.text.unicode>=65&&event.text.unicode<=122)|| (event.text.unicode>=48&&event.text.unicode<=57) ) && text.getString().getSize()<max_size){
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
    pointer.setPosition(pos_x+text.getLocalBounds().width+5,pos_y);
}
//
void Text_Box::Draw(sf::RenderWindow &window){
    window.draw(text);
    if(typing && fmod(pointer_clock.getElapsedTime().asSeconds(),1.0) > 0.5 ){
        window.draw(pointer);
    }
}
