// Contains Main class declaration
#ifndef CURVE_MAIN
#define CURVE_MAIN
// Standard Headers
#include <vector>
// Class Headers
//#include "client.h"
#include "config.h"
#include "game.h"
#include "game_setup.h"
#include "player.h"
#include "server.h"
#include "client.h"
#include "renderer.h"
// Temporary
#include "../functions/functions.h"

//
class Main{
    public:
        // Vars
        Config config;
        Game game;
        Game_Setup game_setup;
        std::vector<Player> player;
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
        void Main_Menu_Handler();
        void Game_Setup_Handler();
        void Change_Button(const int &button,unsigned const int &pl);
        void Play_Handler();
};

#endif // CURVE_MAIN


