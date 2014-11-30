// Contains Main class declaration
#ifndef CURVE_MAIN
#define CURVE_MAIN
// Needed Headers
//#include "..\curve.h"
// Standard headers
//#include <SFML/Graphics.hpp>
//#include <SFML/Network.hpp>
//#include <thread>
//#include <iostream>
//#include <vector>
// Class Headers
//#include "client.h"
#include "config.h"
#include "game.h"
#include "player.h"
#include "server.h"
// Temporary
#include "../defines.h"

//
class Main{
    public:
        // Vars
        Config config;
        Game game;
        Player player[MAX_PLAYERS];
        Server server;
        sf::RenderWindow window;
        // Functions

        // Constructor
        //Main();
        // Functions
        void Curve_Project();
};

#endif // CURVE_MAIN


