// Contains Main class declaration
#ifndef CURVE_MAIN
#define CURVE_MAIN
// Class Headers
//#include "client.h"
#include "config.h"
#include "game.h"
#include "player.h"
#include "server.h"
#include "client.h"
#include "renderer.h"
// Temporary
#include "../defines.h"
#include "../functions/functions.h"

//
class Main{
    public:
        // Vars
        Config config;
        Game game;
        Player player[MAX_PLAYERS];
        Server server;
        Client client;
        Renderer renderer;
        sf::Event event;
        // Functions

        // Constructor
        Main() :renderer(config) {};
        // Functions
        void Curve_Project();
        void Event_Handler();
};

#endif // CURVE_MAIN


