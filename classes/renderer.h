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
//#include <vector>
// Class Headers
#include "config.h"
#include "game.h"
#include "player.h"
//#include "server.h"
#include "client.h"
#include "main_menu.h"
#include "game_setup.h"
#include "game_setup_mp.h"
#include "game_store.h"
#include "powerup_sprite.h"
// Temporary
#include "../defines.h"
#include "../functions/functions.h"

//
class Renderer{
    public:
        // Vars
        sf::RenderWindow window;
        std::thread thread;
        Main_Menu main_menu;
        Game_Setup game_setup;
        Game_Setup_MP game_setup_mp;
        Game_Store game_store;
        Powerup_Sprite sprite;
        // Constructor
        Renderer(const Config &config) : main_menu(config), game_setup(config), game_setup_mp(config), game_store(config), sprite(config){};
        // Functions
        void Start(const Config &config,Game &game,Player player[]);
        void Thread(const Config &config,Game &game,Player player[]);
        void Main_Men(const Config &config,Game &game,Player player[],Main_Menu &main_menu);
        void Server_IP(const Config &config,Game &game,Player player[],Main_Menu &main_menu);
        void Setup_SP(const Config &config,Game &game,Player player[],Game_Setup &game_setup);
        void Setup_MP(const Config &config,Game &game,Player player[],Game_Setup_MP &game_setup);
        void Gam(const Config &config,Game &game,Player player[],Game_Store &game_store);
        void Game_MP(const Config &config,Game &game,Player player[],Game_Store &game_store);
        void PowerUp(const Config &config,Game &game,Powerup_Sprite &sprite);
        void Game_Status_Menu(const Config &config,Game &game, Player player[],Game_Store &game_store);
};

#endif // CURVE_RENDER



