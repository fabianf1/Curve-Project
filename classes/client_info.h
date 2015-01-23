// Contains Game Setup class declaration
#ifndef CURVE_CLIENT_INFO
#define CURVE_CLIENT_INFO
// SFML headers
#include <SFML/Network.hpp>
// Standard headers
#include <vector>

class Client_Info{
    public:
        // Vars
        std::unique_ptr<sf::TcpSocket> socket;
        std::vector<unsigned int> id; // Place in player array
        sf::IpAddress ip;
        bool ready;
        bool connected;
        // Constructor
        Client_Info(): socket(new sf::TcpSocket){
            connected=false;
            ready=false;
        }
};

#endif // CURVE_CIENT_INFO


