// Contains Game setup class declaration
#ifndef CURVE_PENDING
#define CURVE_PENDING
// Standard Headers
#include <vector>
// SFML Headers
#include <SFML/Network.hpp>
// If server send to all id's given
// If client: ignore!
class Pending{
    public:
        // Vars
        sf::Packet packet;
        std::vector<int> sendID;
};

#endif // CURVE_PENDING
