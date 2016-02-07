// Headers
#include "TextBox.h"
#include "main.h"

// TextBox
void TextBox::process(Main &main){
    if(update){
        update(main,*this);
    }
    // Caret
    if(activated){
        // Set
        caret.setPosition(drawable.getPosition().x+drawable.getLocalBounds().width+5,drawable.getPosition().y);
        caret.setColor(normalColor);

    }
    //
    if(check(main.renderer.window)){
        action(main,*this);
    }
}
void TextBox::draw(sf::RenderWindow &window){
    window.draw(drawable);
    if(activated && fmod(caretClock.getElapsedTime().asSeconds(),1.0) > 0.5 ){
        window.draw(caret);
    }
}
TextBox::TextBox(sf::Text &&i_drawable, std::function<bool(Main&,ObjectButton<sf::Text>&)> &&i_visible, std::function<void(Main&,ObjectButton<sf::Text>&)> &&i_action, std::function<void(Main&,ObjectButton<sf::Text>&)> &&i_update, unsigned int &i_id, const sf::Color &i_selectedColor,const unsigned int &i_selectedStyle):
    ObjectButton(std::move(i_drawable), std::move(i_visible), std::move(i_action),std::move(i_update),i_id,i_selectedColor,i_selectedStyle){
    // Initialize caret
    caret.setString("|");
    caret.setFont(*drawable.getFont());
    caret.setCharacterSize(drawable.getCharacterSize());
}
