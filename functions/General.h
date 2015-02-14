// All functions that operate outside of classes
#ifndef CURVE_GENERAL
#define CURVE_GENERAL

// Needed Headers
#include <string>
#include <sstream>
#include <Windows.h> // I really should test on linux
#include <SFML/Graphics.hpp>

std::string int2str(const int &x);
sf::Image LoadImageFromResource(const std::string& name);

#endif // CURVE_GENERAL
