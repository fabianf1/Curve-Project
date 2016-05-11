// Contains Main class declaration
#ifndef CURVE_MAIN
#define CURVE_MAIN
// Standard Headers
#include <vector>
#include "Config.h"
#include "Game.h"
#include "GameSetup.h"
#include "Player.h"
#include "Server.h"
#include "Client.h"
#include "Renderer.h"
#include "Pacer.h"
#include "ObjectStorage.h"

//
class Main{
    public:
        // Vars
        Config config;
        Game game;
        GameSetup gameSetup;
        std::vector<Player> player;
        Server server;
        Client client;
        Renderer renderer;
        ObjectStorage storage;
        sf::Event event;
        Pacer threadPacer;
        // Constructor
        Main() :game(config), storage(config), threadPacer(config.mainThreadMinRate) {};
        // Functions
        void curveProject();
        void eventHandler();
};
//
#endif // CURVE_MAIN


