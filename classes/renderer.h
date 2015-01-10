// Contains Renderer class declaration
#ifndef CURVE_RENDER
#define CURVE_RENDER
// Needed Headers
//#include "..\curve.h"
// Standard headers
#include <SFML/Graphics.hpp>
//#include <SFML/Network.hpp>
#include <thread>
#include <iostream>
#include <vector>
// Class Headers
#include "config.h"
#include "game.h"
#include "player.h"
#include "client.h"
#include "renderer_objects.h"
#include "powerup_sprite.h"
// Temporary
#include "../functions/functions.h"

//
class Renderer{
    public:
        // Vars
        sf::RenderWindow window;
        std::thread thread;
        Renderer_Objects objects;
        Powerup_Sprite sprite;
        // Constructor
        Renderer(const Config &config) : objects(config), sprite(config){};
        // Functions
        void Start(const Config &config,Game &game,const std::vector<Player> &player);
        void Thread(const Config &config,Game &game,const std::vector<Player> &player);
        void Main_Menu();
        void Setup(const Config &config,const Game &game,const std::vector<Player> &player);
        void Play(const Config &config,const Game &game,const std::vector<Player> &player);
        void PowerUp(const Config &config,const Game &game);
        void Shutdown();
};

#endif // CURVE_RENDER



