// Contains Game Setup class declaration
#ifndef CURVE_CLIENT_INFO
#define CURVE_CLIENT_INFO
// SFML headers
#include <SFML/Network.hpp>

class Client_Info{
    public:
        // Vars
        std::unique_ptr<sf::TcpSocket> socket;
        unsigned int id; // Place in player array
        sf::IpAddress ip;
        //int error_count;
        //bool sync;
        bool connected;
        // Constructor
        Client_Info(): socket(new sf::TcpSocket){
            //sync=false;
            connected=false;
            //error_count=0;
        }
};

#endif // CURVE_CIENT_INFO


