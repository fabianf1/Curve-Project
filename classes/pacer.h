// Contains Pacer class declaration
// pace is the minimum run time in milliseconds
#ifndef CURVE_PACER
#define CURVE_PACER
// SFML Headers
#include <SFML/System.hpp>
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


