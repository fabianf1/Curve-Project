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
//#include "server.h"
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
        void Start(const Config &config,Game &game,std::vector<Player> &player);
        void Thread(const Config &config,Game &game,std::vector<Player> &player);
        void Main_Menu(const Config &config,Game &game,std::vector<Player> &player);
        void Setup(const Config &config,Game &game,std::vector<Player> &player);
        void Play(const Config &config,Game &game,std::vector<Player> &player);
        void PowerUp(const Config &config,Game &game);
        void Shutdown();

        //void Setup_SP(const Config &config,Game &game,Player player[],Game_Setup &game_setup);
        //void Setup_MP(const Config &config,Game &game,Player player[],Game_Setup_MP &game_setup);
        //void Gam(const Config &config,Game &game,Player player[],Game_Store &game_store);
        //void Game_MP(const Config &config,Game &game,Player player[],Game_Store &game_store);
        //void PowerUp(const Config &config,Game &game,Powerup_Sprite &sprite);
        //void Game_Status_Menu(const Config &config,Game &game, Player player[],Game_Store &game_store);
};

#endif // CURVE_RENDER



