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
#include "player.h"
#include "client_info.h"
// Temporary
//void Config_Package(Game &game);
#include "../functions/functions.h"
//
class Server{
    public:
        // Vars
        std::vector<Client_Info> clients;
        sf::SocketSelector selector;
        int client_count;
        bool started;
        bool accept_new;
        sf::TcpListener listener;
        sf::Packet packet;
        // Threads
        std::thread thread_listener;
        std::thread thread_sender;
        // Functions
        void Start(const Config &config,Game &game,Player player[]);
        void Server_Listener(const Config &config,Game &game,Player player[]);
        void Server_Sender(const Config &config,Game &game,Player player[]);
        void Client_Init_Packages(const Config &config,Game &game,Player player[]);
        void Process_Package(const Config &config,Game &game,Player player[],sf::Packet &packet);
        void Shutdown();
        void Sync_Clients(const Config &config,Game &game,Player player[]);
        // Constructor
        Server(){
            started=false;
            client_count=0;
            accept_new=true;
        }
        // Functions
};

#endif // CURVE_SERVER

