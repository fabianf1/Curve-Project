// This file contains an enum to keep track of what the packet contains and operator overloads to allow sending it with sf::Packet
#ifndef CURVE_ENUM
#define CURVE_ENUM
// SFML Headers
#include <SFML/Network.hpp>
//
enum class Packet : int {ID,Sync,Name,StartGame,NewRound,Update,Points,GameEnd,RoundEnd,PowerupDeleteField,PowerupDeletePlayer,PowerupSpawn,PowerupHit,PowerupEnd,Pause,Ready,Disconnect,Options,KeyL,KeyR,Lag,Countdown,ReturnSetup,RequestPlayer,RemovePlayer,Version};
// Overload Packer class for this enum
sf::Packet& operator <<(sf::Packet& packet, const Packet &j);
sf::Packet& operator >>(sf::Packet& packet, Packet &j);

#endif // CURVE_ENUM
