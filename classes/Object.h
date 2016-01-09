// Defines the object storage classes
#ifndef CURVE_OBJECT
#define CURVE_OBJECT
// Standard Headers
#include <SFML/Graphics.hpp>
#include "game.h"
#include "PowerupSprite.h"
#include "PowerupField.h"
// Forward declaration
class Main;

// CANNOT SPLIT HEADER AND IMPLEMENATION!!!
//http://stackoverflow.com/questions/12573816/what-is-an-undefined-reference-unresolved-external-symbol-error-and-how-do-i-fix/12574417#12574417
// Can do using explicit instantiation
// http://stackoverflow.com/questions/495021/why-can-templates-only-be-implemented-in-the-header-file?answertab=votes#tab-top
//

// Base class. Defines the functions and variables that need to be accessed from the outside
class ObjectBase{
public:
    virtual bool checkVisible(Main &main) = 0;
    virtual void process(Main &main){
        return;
    }
    virtual void draw(sf::RenderWindow &window) = 0;
};

// Static template class
template <class Drawable>
class ObjectStatic : public ObjectBase{
public:
    Drawable drawable;
    std::function<bool(Main&,ObjectStatic<Drawable>&)> visible; // Change this to true function
    std::function<void(Main&,ObjectStatic<Drawable>&)> update;
    unsigned int updateInfo;
    // Draw function
    void draw(sf::RenderWindow &window);
    virtual bool checkVisible(Main &main);
    virtual void process(Main &main);
    // Constructor; Copying is required!
    ObjectStatic(Drawable &&i_drawable, std::function<bool(Main&,ObjectStatic<Drawable>&)> &&i_visible);
    ObjectStatic(Drawable &&i_drawable, std::function<bool(Main&,ObjectStatic<Drawable>&)> &&i_visible,std::function<void(Main&,ObjectStatic<Drawable>&)> &&i_update);
    ObjectStatic(Drawable &&i_drawable, std::function<bool(Main&,ObjectStatic<Drawable>&)> &&i_visible,std::function<void(Main&,ObjectStatic<Drawable>&)> &&i_update, unsigned int &&i_updateInfo);
};

// Button template class
template <class Drawable>
class ObjectButton : public ObjectBase{
public: // Should move
    Drawable drawable;
    std::function<bool(Main&,ObjectButton<Drawable>&)> visible; // Change this to true function
    std::function<void(Main&,ObjectButton<Drawable>&)> action;
    std::function<void(Main&,ObjectButton<Drawable>&)> update;
    sf::Clock lastClick;
    unsigned int normalStyle;
    unsigned int selectedStyle;
    unsigned int activatedStyle;
    sf::Color normalColor;
    sf::Color selectedColor;
    bool activated;
    unsigned int updateInfo;
    // Draw function
    virtual void draw(sf::RenderWindow &window);
    virtual bool checkVisible(Main &main);
    virtual void process(Main &main);
    //virtual void update(Main &main);
    bool check(sf::RenderWindow &window);
    // Constructor; Copying is required!
    ObjectButton(Drawable &&i_drawable, std::function<bool(Main&,ObjectButton<Drawable>&)> &&i_visible, std::function<void(Main&,ObjectButton<Drawable>&)> &&i_action,const sf::Color &i_selectedColor,const unsigned int &i_selectedStyle);
    ObjectButton(Drawable &&i_drawable, std::function<bool(Main&,ObjectButton<Drawable>&)> &&i_visible, std::function<void(Main&,ObjectButton<Drawable>&)> &&i_action, std::function<void(Main&,ObjectButton<Drawable>&)> &&i_update, unsigned int &i_updateInfo, const sf::Color &i_selectedColor,const unsigned int &i_selectedStyle);
};

// TextBox class
class TextBox : public ObjectButton<sf::Text>{
public:
    sf::Clock caretClock;
    sf::Text caret;
    virtual void process(Main &main);
    virtual void draw(sf::RenderWindow &window);
    TextBox(sf::Text &&i_drawable, std::function<bool(Main&,ObjectButton<sf::Text>&)> &&i_visible, std::function<void(Main&,ObjectButton<sf::Text>&)> &&i_action, std::function<void(Main&,ObjectButton<sf::Text>&)> &&i_update, unsigned int &i_updateInfo, const sf::Color &i_selectedColor,const unsigned int &i_selectedStyle);
};
//
class Line : public ObjectBase{
public:
    bool checkVisible(Main &main);
    void draw(sf::RenderWindow &window); // Will need more info for darkness; Save pointer to player?
    sf::VertexArray *line;
    sf::VertexArray *noTurtleLine;
    unsigned int id;
    Line(sf::VertexArray *i_line, sf::VertexArray *i_noTurtleLine, unsigned int &i_id);
};
//
class Head : public ObjectBase{
public:
    bool checkVisible(Main &main);
    void draw(sf::RenderWindow &window); // Will need more info for darkness
    sf::CircleShape *circle;
    sf::RectangleShape *rectangle;
    bool *death;
    bool *rightAngle;
    unsigned int id;
    Head(sf::CircleShape *i_circle, sf::RectangleShape *i_rectangle, bool *i_death, bool *i_rightAngle, unsigned int &i_id);
};
//
class PowerupObjects : public ObjectBase{
public:
    bool checkVisible(Main &main);
    void draw(sf::RenderWindow &window);
    std::vector<PowerupField> *powerups;
    PowerupSprite sprite;
    const Config *config;
    PowerupObjects(const Config &i_config, std::vector<PowerupField> *i_powerups);
};
//

#endif // CURVE_OBJECT
