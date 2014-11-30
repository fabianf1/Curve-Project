// Contains Game Setup class declaration
#ifndef CURVE_SERVER
#define CURVE_SERVER
// Needed Headers
//#include "..\curve.h"
// Standard headers
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <thread>
#include <iostream>
#include <vector>
// Class Headers
#include "client.h"
// Temporary
void Config_Package(Game &game);
//
class Server{
    public:
        // Vars
        std::vector<Client> clients;
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
        void Start_Server(const Config &config,Game &game,Player player[]);
        void Server_Listener(const Config &config,Game &game,Player player[]);
        void Server_Sender(const Config &config,Game &game,Player player[]);
        void Client_Init_Packages(const Config &config,Game &game,Player player[]);
        void Process_Package(const Config &config,Game &game,Player player[],sf::Packet &packet);
        void Shutdown();
        // Constructor
        Server(){
            started=false;
            client_count=0;
            accept_new=true;
        }
        // Functions
};

#endif // CURVE_SERVER

