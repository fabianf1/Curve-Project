// This class is used to store the different powerups in the game. As each powerup has a different spawn rate, disappear time etc. is is convenient to have such a class.
#ifndef CURVE_POWERUP
#define CURVE_POWERUP
// SFML Headers
#include <SFML/Network.hpp>
//
class Powerup{
    public:
        // Enums vars
        enum class Type : int {Slow,Fast,Small,Big,RightAngle,Clear,Invisible,WallsAway,MorePowerups,InvertKeys,QuestionMark,Darkness,Gap,Bomb,Sine} type;
        enum class Impact : int {Self,Other,All} impact;
        // Vars
        int spawnChance; // Overal spawn chance depends on the other powerups
        float effectMinDisappear; // In seconds
        int effectRandDisappear; // In milliseconds
        // Functions
        // Constructor
        Powerup(const Powerup::Type &Type,const Powerup::Impact &Impact, const int &Chance, const float &Min, const int &Rand);
};
// Overloads for sf::Packet
sf::Packet& operator << (sf::Packet& packet, const Powerup::Type &j);
sf::Packet& operator >> (sf::Packet& packet, Powerup::Type &j);
sf::Packet& operator << (sf::Packet& packet, const Powerup::Impact &j);
sf::Packet& operator >> (sf::Packet& packet, Powerup::Impact &j);
//
#endif // CURVE_POWERUP
