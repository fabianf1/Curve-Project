// Contains Game setup class declaration
#ifndef CURVE_SERVER
#define CURVE_SERVER
// Standard headers
#include <thread>
#include <iostream>
#include <vector>
#include <mutex>
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include "Config.h"
#include "Game.h"
#include "GameSetup.h"
#include "Player.h"
#include "ClientInfo.h"
#include "Color.h"
#include "Pacer.h"

#include "../functions/general.h"
//
class Server{
    public:
        // Vars
        std::vector<ClientInfo> clients;
        std::mutex clientMutex;
        sf::SocketSelector selector;
        sf::TcpListener listener;
        sf::Packet packet;
        Pacer senderPacer;
        // Threads
        std::thread thread_listener;
        std::thread thread_sender;
        // Functions
        void start(const Config &config,GameSetup &gameSetup,Game &game,std::vector<Player> &player);
        void serverListener(const Config &config,GameSetup &gameSetup,Game &game,std::vector<Player> &player);
        void serverSender(const Config &config,Game &game,std::vector<Player> &player);
        void newClient(const Config &config,GameSetup &gameSetup,Game &game,std::vector<Player> &player, const unsigned int &n, const bool &new_client);
        void disconnectClient(GameSetup &gameSetup, Game &game,std::vector<Player> &player, const unsigned int &n);
        void processPackage(const Config &config,GameSetup &gameSetup,Game &game,std::vector<Player> &player,sf::Packet &packet,const unsigned int &n);
        void shutdown(Game &game);
        void syncClients(Game &game,const std::vector<Player> &player);
        void updatePlayerID(std::vector<Player> &player,const unsigned int &n);
        // Constructor
        Server();
};

#endif // CURVE_SERVER

