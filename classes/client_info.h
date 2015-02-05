// Contains Game Setup class declaration
#ifndef CURVE_CLIENT_INFO
#define CURVE_CLIENT_INFO
// SFML headers
#include <SFML/Network.hpp>
// Standard headers
#include <vector>
#include <memory>

class Client_Info{
    public:
        // Vars
        std::unique_ptr<sf::TcpSocket> socket;
        std::vector<unsigned int> id; // Place in player array
        bool ready;
        bool version_correct;
        // Constructor
        Client_Info();
        // Function
        bool Check_ID(const unsigned int &n);
};

#endif // CURVE_CIENT_INFO


