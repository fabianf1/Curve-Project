// Contains Pacer class declaration
// minRunTime is the minimum run time in milliseconds
#ifndef CURVE_PACER
#define CURVE_PACER
// Headers
#include <SFML/System.hpp>
//
class Pacer{
    // Vars
    sf::Clock clock;
    sf::Time minRunTime;
    public:
        // Functions
        void setPace(const unsigned int &i_minRunRate);
        unsigned int getPace();
        void resetClock();
        void pace();
        // Constructor
        Pacer(const unsigned int &i_minRunRate);
};

#endif // CURVE_PACER


