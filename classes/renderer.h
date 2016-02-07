// Contains Renderer class declaration
#ifndef CURVE_RENDER
#define CURVE_RENDER
//Headers
#include <thread>
#include <iostream>
#include <vector>
#include <memory>
#include <SFML/Graphics.hpp>
#include "Config.h"
#include "Game.h"
#include "Player.h"
#include "Client.h"
#include "ObjectBase.h"
//
class Renderer{
    public:
        // Vars
        sf::RenderWindow window;
        std::thread renderThread;
        int fps;
        // Functions
        void start(Main &main, const Config &config,Game &game,std::vector<std::unique_ptr<ObjectBase>> &objects, const std::vector<Player> &player);
        void thread(Main &main, const Config &config,Game &game,std::vector<std::unique_ptr<ObjectBase>> &objects, const std::vector<Player> &player);
        void shutdown();
};

#endif // CURVE_RENDER



