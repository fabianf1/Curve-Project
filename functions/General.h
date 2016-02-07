// All functions that operate outside of classes
#ifndef CURVE_GENERAL
#define CURVE_GENERAL

// Needed Headers
#include <Windows.h> // I really should test on linux
#include <SFML/Graphics.hpp>
//
sf::Image LoadImageFromResource(const std::string& name);
// Needs better place maybe?
const char* getKeyName(const sf::Keyboard::Key &key );

#endif // CURVE_GENERAL
