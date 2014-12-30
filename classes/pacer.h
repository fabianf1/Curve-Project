// Contains Pacer class declaration
// Pace is the minimum run time in milliseconds
#ifndef CURVE_PACER
#define CURVE_PACER
// Standard headers

// SFML Headers
#include <SFML/System.hpp>
// Class Headers

//
class Pacer{
    // Vars
    sf::Clock clock;
    unsigned int pace;
    public:
        // Functions
        void setPace(const unsigned int &Pace);
        unsigned int getPace();
        void resetClock();
        void Pace();
        // Constructor
        Pacer(const unsigned int &Pace);
};

#endif // CURVE_PACER


