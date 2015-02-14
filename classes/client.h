// Contains Client class declaration
#ifndef CURVE_CLIENT
#define CURVE_CLIENT
// Headers
#include <iostream>
#include <thread>
#include <SFML/Network.hpp>
#include "Config.h"
#include "Game.h"
#include "Player.h"
#include "Pending.h"
#include "Color.h"
#include "Powerup.h"
#include "Pacer.h"
#include "Packet.h"
//
class Client{
    public:
        // Vars
        std::thread clientThread;
        sf::TcpSocket socket;
        bool sync;
        bool ready;
        sf::Packet packet;
        Pacer pacer;
        // Functions
        void start(const Config &config, Game &game,std::vector<Player> &player);
        void thread(const Config &config,Game &game,std::vector<Player> &player);
        void toggleReady(Game &game,std::vector<Player> &player);
        void processPacket(const Config &config,Game &game,std::vector<Player> &player,sf::Packet &packet);
        void syncPackage(Game &game,std::vector<Player> &player,sf::Packet &packet);
        void shutdown(Game &game);
        // Constructor
        Client();
};
#endif // CURVE_CLIENT
//
