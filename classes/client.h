// Contains Client class declaration
#ifndef CURVE_CLIENT
#define CURVE_CLIENT
// Standard Headers
#include <iostream>
#include <thread>
// SFML Headers
#include <SFML/Network.hpp>
// Class Headers
#include "config.h"
#include "game.h"
#include "player.h"
#include "pending.h"
#include "color.h"
// Temporary
#include "../functions/functions.h"
//
class Client{
    public:
        // Vars
        std::thread thread;
        sf::TcpSocket socket;
        bool connected;
        bool sync;
        bool ready;
        sf::Packet packet;
        // Functions
        void Start(const Config &config, Game &game,std::vector<Player> &player);
        void Thread(const Config &config,Game &game,std::vector<Player> &player);
        void Ready(Game &game,std::vector<Player> &player);
        void Process_Packet(const Config &config,Game &game,std::vector<Player> &player,sf::Packet &packet);
        void Shutdown(Game &game);
        // Constructor
        //Client();
};
#endif // CURVE_CLIENT
//
