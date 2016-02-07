// Defines the object storage classes
#ifndef CURVE_OBJECTSTORAGE
#define CURVE_OBJECTSTORAGE
// Standard Headers
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <memory>
#include <mutex>
#include "config.h"
#include "game.h"
#include "ObjectStatic.h"
#include "ObjectButton.h"
#include "ObjectHead.h"
#include "ObjectLine.h"
#include "ObjectPowerups.h"
#include "TextBox.h"
// Forward declaration
class Main;
//
class ObjectStorage{
public:
    // Variables
    std::vector<std::unique_ptr<ObjectBase>> objects;
    const sf::Font font;
    Game::Mode mode;
    // Constructor
    ObjectStorage(const Config &config);
    // Functions
    void initMainMenu(const Config &config);
    void initGameSetup(const Config &config);
    void initGame(const Config &config, Game &game, std::vector<Player> &player);
    // Helpers
    sf::Font loadFont(const std::string& name);
    // Super Static
    template<class T>
    void addToVector(T &object);
    // Super hideable Static
    template<class T>
    void addToVector(T &object,std::function<bool(Main&,ObjectStatic<T>&)> &visible);
    // Updatable static
    template<class T>
    void addToVector(T &object,std::function<bool(Main&,ObjectStatic<T>&)> &visible,std::function<void(Main&,ObjectStatic<T>&)> &update);
    template<class T>
    void addToVector(T &object,std::function<bool(Main&,ObjectStatic<T>&)> &visible,std::function<void(Main&,ObjectStatic<T>&)> &update, unsigned int &id);
    // Button without update action
    template<class T>
    void addToVector(T &object, std::function<bool(Main&,ObjectButton<T>&)> &visible, std::function<void(Main&,ObjectButton<T>&)> &action,const sf::Color &selectedColor,const unsigned int &selectedStyle);
    // Complete button
    template<class T>
    void addToVector(T object, std::function<bool(Main&,ObjectButton<T>&)> &visible, std::function<void(Main&,ObjectButton<T>&)> &action, std::function<void(Main&,ObjectButton<T>&)> &update, unsigned int &id,const sf::Color &selectedColor,const unsigned int &selectedStyle);
    // TextBox
    void addToVector2(sf::Text object, std::function<bool(Main&,ObjectButton<sf::Text>&)> &visible, std::function<void(Main&,ObjectButton<sf::Text>&)> &action, std::function<void(Main&,ObjectButton<sf::Text>&)> &update, unsigned int &id,const sf::Color &selectedColor,const unsigned int &selectedStyle);
};
#endif // CURVE_OBJECTSTORAGE
