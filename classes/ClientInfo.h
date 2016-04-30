// Contains Game setup class declaration
#ifndef CURVE_CLIENTINFO
#define CURVE_CLIENTINFO
// SFML headers
#include <vector>
#include <SFML/Network.hpp>

class ClientInfo{
    public:
        // Vars
        std::unique_ptr<sf::TcpSocket> socket;
        std::vector<unsigned int> id; // Place in player array
        bool ready;
        bool versionCorrect;
        // Constructor
        ClientInfo();
        // Function
        bool checkID(const unsigned int &n);
        void updateID(const unsigned int &n);
};

#endif // CURVE_CLIENTINFO


