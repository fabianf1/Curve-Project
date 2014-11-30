// Contains Game Setup class declaration
#ifndef CURVE_PENDING
#define CURVE_PENDING
// Needed Headers
#include "..\curve.h"
// If server send to all id's given
// If client: ignore!
class Pending{
    public:
        // Vars
        sf::Packet packet;
        std::vector<int> send_id;
};

#endif // CURVE_PENDING
