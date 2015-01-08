// This class is used to store the different powerups in the game. As each powerup has a different spawn rate, disappear time etc. is is convenient to have such a class.

#ifndef CURVE_POWERUP
#define CURVE_POWERUP
// SFML Headers
#include <SFML/Network.hpp>
// Standard Headers

// Class Headers

//
class Powerup{
    public:
        // Enums vars
        enum class Type : int {Slow,Fast,Small,Big,Right_Angle,Clear,Invisible,Walls_Away,More_Powerups,Invert_Keys,Question_Mark,Darkness,Gap,Bomb,Sine} type;
        enum class Impact : int {Self,Other,All} impact;
        // Vars
        int spawn_chance; // Overal spawn chance depends on the other powerups
        int effect_min_disappear; // In seconds
        int effect_rand_disappear; // In milliseconds
        // Functions
        // Constructor
        Powerup(const Powerup::Type &Type,const Powerup::Impact &Impact, const int &Chance, const int &Min, const int &Rand);
};
// Overloads for sf::Packet
sf::Packet& operator << (sf::Packet& packet, const Powerup::Type &j);
sf::Packet& operator >> (sf::Packet& packet, Powerup::Type &j);
sf::Packet& operator << (sf::Packet& packet, const Powerup::Impact &j);
sf::Packet& operator >> (sf::Packet& packet, Powerup::Impact &j);
//
#endif // CURVE_POWERUP
