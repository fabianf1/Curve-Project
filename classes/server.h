// Contains Game Setup class declaration
#ifndef CURVE_SERVER
#define CURVE_SERVER
// Needed Headers
//#include "..\curve.h"
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <thread>
#include <iostream>
#include <vector>
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

        // Constructor
        Server(){
            started=false;
            client_count=0;
            accept_new=true;
        }
        // Functions
};

#endif // CURVE_SERVER

