// All functions that operate outside of classes
#ifndef CURVE_FUNCTIONS
#define CURVE_FUNCTIONS

// Needed Headers
#include <SFML/Graphics.hpp>
#include <sstream>
#include <Windows.h> // I really should test on linux

// General.cpp
sf::String int2string(int x);
void Sleep(const int &frame_time,sf::Clock &clock);
sf::Image LoadImageFromResource(const std::string& name);

#endif // CURVE_FUNCTIONS
