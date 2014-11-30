// Contains Game Setup class declaration
#ifndef CURVE_CLIENT
#define CURVE_CLIENT
// Needed Headers
#include "..\curve.h"

class Client{
    public:
        // Vars
        std::unique_ptr<sf::TcpSocket> socket;
        int id; // Place in player array
        sf::IpAddress ip;
        //int error_count;
        //bool sync;
        bool connected;
        // Constructor
        Client(): socket(new sf::TcpSocket){
            //sync=false;
            connected=false;
            //error_count=0;
        }
};

#endif // CURVE_CIENT


