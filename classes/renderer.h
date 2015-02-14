// Contains Renderer class declaration
#ifndef CURVE_RENDER
#define CURVE_RENDER
//Headers
#include <thread>
#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
#include "Config.h"
#include "Game.h"
#include "Player.h"
#include "Client.h"
#include "rendererObjects.h"
#include "PowerupSprite.h"
//
#include "../functions/general.h"

//
class Renderer{
    public:
        // Vars
        sf::RenderWindow window;
        std::thread renderThread;
        RendererObjects objects;
        PowerupSprite sprite;
        // Constructor
        Renderer(const Config &config) : objects(config), sprite(config){};
        // Functions
        void start(const Config &config,Game &game,const std::vector<Player> &player);
        void thread(const Config &config,Game &game,const std::vector<Player> &player);
        void mainMenu();
        void setup(const Config &config,const Game &game,const std::vector<Player> &player);
        void play(const Config &config,const Game &game,const std::vector<Player> &player);
        void powerUp(const Config &config,const Game &game,const std::vector<Player> &player);
        void shutdown();
};

#endif // CURVE_RENDER



