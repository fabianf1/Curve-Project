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
        sf::Event event;
        Pacer threadPacer;
        // Constructor
        Main() :game(config), renderer(config),threadPacer(config.mainThreadMinRate) {};
        // Functions
        void curveProject();
        void eventHandler();
        void mainMenuHandler();
        void gameSetupHandler();
        void changeButton(const int &button,unsigned const int &pl);
        void gameHandler();
};

#endif // CURVE_MAIN


