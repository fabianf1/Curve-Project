// Contains functions and constructors for the Text_Button class
// Needed Header
#include "..\curve.h"
// Functions
void Prompt::setPrompt(int number,sf::String fields[],sf::Font &font,int fontsize,int x, int y){
    //
    int width=fontsize*(10)
    int height=fontsize*(2+number*3);
    //
    border.setSize(sf::Vector2f(width,height));
    border.setOutlineColor(sf::Color::Red);
    border.setOutlineThickness(1);
    border.setPosition(x-width,y-height);
    //
    for(int i=0;i<number;i++){
        sf::Text text(fields[0],font,fontsize);
        text.setPosition(x-width/2+10,y-height/2+0.5*fontsize);
        field_text.push_back(text);
        //
        sf::RectangleShape rect(sf::Vector2f(width/2-20,fontsize*1.5));
        rect.setPosition(x+5,y-height/2+0.25*fontsize);
        field_border.push_back(rect);
        //
        text.setString();
        text.setPosition(y-height/2+0.5*fontsize)
    }
    //
    confirm.setButton("Confirm",font,fontsize,sf::Color::Red,sf::Color::Yellow,sf::Text::Regular,sf::Text::Italic);
    confirm.setPosition(x-width/2-confirm.getLocalBounds().width/2,y+height/2-fontsize*1.5);
}
//
void Prompt::Empty(){
    for(unsigned i=0;i<field_text.size(),i++){
        field_text[i].setString("");
    }
}
