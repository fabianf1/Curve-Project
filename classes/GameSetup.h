// Contains Game setup class declaration
#ifndef CURVE_GAMESETUP
#define CURVE_GAMESETUP
// Headers
#include <vector>
#include <iostream>
#include <SFML/Graphics.hpp>
#include "Config.h"
#include "Game.h"
#include "Player.h"
// Prototype
class Server;
//
class GameSetup{
    public:
        // Variables
        sf::Color color[8];
        bool colorUsed[8];
        // Constructor
        GameSetup();
        // Functions
        void initialize(const Config &config,Game &game,std::vector<Player> &player);
        void addPlayer(Game &game,std::vector<Player> &player);
        void removePlayer(Game &game,std::vector<Player> &player, const int &i);
        bool keyAvailable(const std::vector<Player> &player,const sf::Keyboard::Key &key);
        void quit(const Config &config,Game &game,std::vector<Player> &player);
        bool startGame(const Config &config,Game &game,std::vector<Player> &player,const Server &server);
        void autoAddPlayers(const Config &config,Game &game,std::vector<Player> &player);
};
// Class headers
#include "server.h"

#endif // CURVE_GAMESETUP
