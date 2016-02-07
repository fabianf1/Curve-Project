// Headers
#include "ObjectButton.h"
#include "main.h"

// Implemenentation of ObjectButton
template <class Drawable>
void ObjectButton<Drawable>::draw(sf::RenderWindow &window){
    window.draw(drawable);
}
template <class Drawable>
bool ObjectButton<Drawable>::checkVisible(Main &main){
    return visible(main,*this);
}
template <class Drawable>
void ObjectButton<Drawable>::process(Main &main){
    if(update){
        update(main,*this);
    }
    if(check(main.renderer.window)){
        action(main,*this);
    }
}
template <class Drawable>
bool ObjectButton<Drawable>::check(sf::RenderWindow &window){
    // check selected
    if(drawable.getGlobalBounds().contains(sf::Mouse::getPosition(window).x,sf::Mouse::getPosition(window).y)){
        drawable.setColor(selectedColor);
        if(!activated){
            drawable.setStyle(selectedStyle);
        }
        else{
            drawable.setStyle(selectedStyle|activatedStyle);
        }
        if(window.hasFocus()&&sf::Mouse::isButtonPressed(sf::Mouse::Left) && lastClick.getElapsedTime().asSeconds()>0.15){
            lastClick.restart();
            return true;
        }
    }
    else{
        drawable.setColor(normalColor);
        if(!activated){
            drawable.setStyle(normalStyle);
        }
        else{
            drawable.setStyle(normalStyle|activatedStyle);
        }
    }
    return false;
}
// Constructor
template <class Drawable>
ObjectButton<Drawable>::ObjectButton(Drawable &&i_drawable, std::function<bool(Main&,ObjectButton<Drawable>&)> &&i_visible, std::function<void(Main&,ObjectButton<Drawable>&)> &&i_action,const sf::Color &i_selectedColor,const unsigned int &i_selectedStyle){
    action=i_action;
    visible=i_visible;
    drawable=i_drawable;
    selectedColor=i_selectedColor;
    selectedStyle=i_selectedStyle;
    normalColor=drawable.getColor();
    normalStyle=drawable.getStyle();
    activated=false;
    activatedStyle=sf::Text::Bold|sf::Text::Underlined;
}
template <class Drawable>
ObjectButton<Drawable>::ObjectButton(Drawable &&i_drawable, std::function<bool(Main&,ObjectButton<Drawable>&)> &&i_visible, std::function<void(Main&,ObjectButton<Drawable>&)> &&i_action, std::function<void(Main&,ObjectButton<Drawable>&)> &&i_update, unsigned int &i_id, const sf::Color &i_selectedColor,const unsigned int &i_selectedStyle):
    ObjectButton(std::move(i_drawable), std::move(i_visible), std::move(i_action), i_selectedColor,i_selectedStyle){
    update=i_update;
    id=i_id;
}
// Explicit instantiation
template class ObjectButton<sf::Text>;
