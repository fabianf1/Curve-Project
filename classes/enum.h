#ifndef CURVE_ENUM
#define CURVE_ENUM

#include <SFML/Network.hpp>

enum class Packet : int {ID,Sync,Name,StartGame,NewRound,Update,Update2,Points,GameEnd,RoundEnd,PowerupDelF,PowerupDelP,PowerupDelG,PowerupS,PowerupHit,Pause,Ready,DCon,Config,KeyL,KeyR,Lag,Countdown};
// Overload Packer class for this enum
sf::Packet& operator <<(sf::Packet& packet, const Packet &j);
sf::Packet& operator >>(sf::Packet& packet, Packet &j);

#endif // CURVE_ENUM
