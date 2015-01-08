// Contains Game Setup class declaration
#ifndef CURVE_SERVER
#define CURVE_SERVER
// Needed Headers
//#include "..\curve.h"
// Standard headers
#include <thread>
#include <iostream>
#include <vector>
// SFML Headers
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
// Class Headers
#include "config.h"
#include "game.h"
#include "game_setup.h"
#include "player.h"
#include "client_info.h"
#include "color.h"
#include "pacer.h"
//
#include "../functions/functions.h"
//
class Server{
    public:
        // Vars
        std::vector<Client_Info> clients;
        sf::SocketSelector selector;
        sf::TcpListener listener;
        sf::Packet packet;
        Pacer sender_pacer;
        // Threads
        std::thread thread_listener;
        std::thread thread_sender;
        // Functions
        void Start(const Config &config,Game_Setup &game_setup,Game &game,std::vector<Player> &player);
        void Server_Listener(const Config &config,Game_Setup &game_setup,Game &game,std::vector<Player> &player);
        void Server_Sender(const Config &config,Game &game,std::vector<Player> &player);
        void New_Client(const Config &config,Game_Setup &game_setup,Game &game,std::vector<Player> &player);
        void Process_Package(const Config &config,Game &game,std::vector<Player> &player,sf::Packet &packet,const unsigned int &n);
        void Shutdown(Game &game);
        void Sync_Clients(const Config &config,Game &game,std::vector<Player> &player);
        // Constructor
        Server(): sender_pacer(25){}
        // Functions
};

#endif // CURVE_SERVER

