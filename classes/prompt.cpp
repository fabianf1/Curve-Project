// Contains functions and constructors for the Text_Button class
// Needed Header
#include "prompt.h"
// Functions
void Prompt::setPrompt(const sf::String &field,const sf::Font &font,const unsigned int &fontsize,const int &X,const int &Y){
    x=X;
    y=Y;
    //
    int width=fontsize*(15);
    int height=fontsize*(5);
    //
    border.setSize(sf::Vector2f(width,height));
    border.setOutlineColor(sf::Color::Red);
    border.setOutlineThickness(1);
    border.setPosition(x-width/2,y-height/2);
    border.setFillColor(sf::Color::Black);
    // Title
    title.setString(field);
    title.setFont(font);
    title.setCharacterSize(fontsize);
    title.setColor(sf::Color::White);
    title.setStyle(sf::Text::Bold);
    title.setPosition(x-title.getLocalBounds().width/2 , y-height/2+fontsize*0.5 );
    // Text field

    text.setString("");
    text.setFont(font);
    text.setCharacterSize(fontsize);
    text.setPosition(x-text.getLocalBounds().width/2,y-fontsize*0.5);
    //
    confirm.setButton("Confirm",font,fontsize,sf::Color::Red,sf::Color::Yellow,sf::Text::Regular,sf::Text::Italic);
    confirm.setPosition(x-confirm.getLocalBounds().width/2,y+height/2-fontsize*1.5);
}
//
void Prompt::Empty(){
    text.setString("");
}
//
void Prompt::Draw(sf::RenderWindow &window){
    if(enabled){
        window.draw(border);
        window.draw(title);
        text.setPosition(x-text.getLocalBounds().width/2,text.getPosition().y);
        window.draw(text);
        window.draw(confirm);
    }
}
//
void Prompt::Event(sf::Event &event){
    //
    if(event.type==sf::Event::TextEntered&&( (event.text.unicode>=48&&event.text.unicode<=57)||event.text.unicode==46) && text.getString().getSize()<16){
        text.setString(text.getString()+static_cast<char>(event.text.unicode));
    }
    else if(event.type==sf::Event::KeyPressed&&event.key.code == sf::Keyboard::BackSpace&&text.getString().getSize()>0){
        sf::String temp=text.getString();
        temp.erase(temp.getSize()-1,1);
        text.setString(temp);
    }
}
//
void Prompt::Switch(){
    enabled=!enabled;
}
//
bool Prompt::getEnabled(){
    return enabled;
}
//
bool Prompt::Check(sf::RenderWindow &window){
    return confirm.Check(window);
}
//
void Prompt::setString(const sf::String &string){
    text.setString(string);
}
//
sf::String Prompt::getString(){
    return text.getString();
}
//
