// Headers
#include "ObjectStatic.h"
#include "main.h"
// Implementation of ObjectStatic
template <class Drawable>
void ObjectStatic<Drawable>::draw(sf::RenderWindow &window){
    window.draw(drawable);
}
template <class Drawable>
bool ObjectStatic<Drawable>::checkVisible(Main &main){
    return visible(main,*this);
}
template <class Drawable>
void ObjectStatic<Drawable>::process(Main &main){
    if(update){
        update(main,*this);
    }
}
template <class Drawable>
ObjectStatic<Drawable>::ObjectStatic(Drawable &i_drawable, std::function<bool(Main&,ObjectStatic<Drawable>&)> &&i_visible){
    visible=i_visible;
    drawable=i_drawable;
}
template <class Drawable>
ObjectStatic<Drawable>::ObjectStatic(Drawable &i_drawable, std::function<bool(Main&,ObjectStatic<Drawable>&)> &&i_visible,std::function<void(Main&,ObjectStatic<Drawable>&)> &&i_update){
    visible=i_visible;
    drawable=i_drawable;
    update=i_update;
}
template <class Drawable>
ObjectStatic<Drawable>::ObjectStatic(Drawable &i_drawable, std::function<bool(Main&,ObjectStatic<Drawable>&)> &&i_visible,std::function<void(Main&,ObjectStatic<Drawable>&)> &&i_update, unsigned int &i_id){
    visible=i_visible;
    drawable=i_drawable;
    update=i_update;
    id=i_id;
}
// Explicit instantiation
template class ObjectStatic<sf::Text>;
template class ObjectStatic<sf::RectangleShape>;
