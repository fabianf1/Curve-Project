#ifndef CURVE_OBJECTSTATIC
#define CURVE_OBJECTSTATIC
// Standard Headers
#include <SFML/Graphics.hpp>
#include "ObjectBase.h"

// Static template class
template <class Drawable>
class ObjectStatic : public ObjectBase{
    std::function<bool(Main&,ObjectStatic<Drawable>&)> visible;
    std::function<void(Main&,ObjectStatic<Drawable>&)> update;
public:
    Drawable drawable;
    unsigned int id;
    void draw(sf::RenderWindow &window);
    virtual bool checkVisible(Main &main);
    virtual void process(Main &main);
    // Constructor
    ObjectStatic(Drawable &i_drawable, std::function<bool(Main&,ObjectStatic<Drawable>&)> &&i_visible);
    ObjectStatic(Drawable &i_drawable, std::function<bool(Main&,ObjectStatic<Drawable>&)> &&i_visible,std::function<void(Main&,ObjectStatic<Drawable>&)> &&i_update);
    ObjectStatic(Drawable &i_drawable, std::function<bool(Main&,ObjectStatic<Drawable>&)> &&i_visible,std::function<void(Main&,ObjectStatic<Drawable>&)> &&i_update, unsigned int &i_id);
};

#endif // CURVE_OBJECTSTATIC
