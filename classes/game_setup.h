// Contains Game Setup class declaration
#ifndef CURVE_GAME_SETUP
#define CURVE_GAME_SETUP
// SFML Headers
#include <SFML/Graphics.hpp>
// Standard headers
#include <vector>
#include <iostream>
// Class Headers
#include "config.h"
#include "game.h"
#include "player.h"

//
class Game_Setup{
    public:
        // Variables
        sf::Color color[6];
        bool color_used[6];
        //
        int key_change[2]; // [0] for key: 0 is left, 1 is right; [1] for player
        // Constructor
        Game_Setup();
        // Functions
        void Initialize(const Config &config,Game &game,std::vector<Player> &player);
        void Add_Player(Game &game,std::vector<Player> &player);
        void Remove_Player(Game &game,std::vector<Player> &player, const int &i);
        bool Key_Available(const std::vector<Player> &player,const sf::Keyboard::Key &key);
        void Quit(const Config &config,Game &game,std::vector<Player> &player);
        void Start_Game(const Config &config,Game &game,std::vector<Player> &player);
        void Auto_Add_Players(const Config &config,Game &game,std::vector<Player> &player);
};

#endif // CURVE_GAME_SETUP
