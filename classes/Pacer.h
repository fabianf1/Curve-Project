// Contains Pacer class declaration
// minPace is the number of iterations per second
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
        void setPace(const unsigned int &i_minPace);
        unsigned int getPace();
        void resetClock();
        void pace();
        // Constructor
        Pacer(const unsigned int &i_minPace);
};

#endif // CURVE_PACER


