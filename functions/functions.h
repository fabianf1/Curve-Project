// All functions that operate outside of classes
#ifndef CURVE_FUNCTIONS
#define CURVE_FUNCTIONS

// Needed Headers
#include <string>
#include <SFML/Graphics.hpp>
#include <sstream>
#include <Windows.h> // I really should test on linux

// General.cpp
std::string int2string(const int &x);
sf::Image LoadImageFromResource(const std::string& name);

#endif // CURVE_FUNCTIONS
