// Contains Client class declaration
#ifndef CURVE_CLIENT
#define CURVE_CLIENT
// Standard Headers
#include <iostream>
#include <thread>
// SFML Headers
#include <SFML/Network.hpp>
// Class Headers
#include "config.h"
#include "game.h"
#include "player.h"
#include "pending.h"
// Temporary
#include "../functions/functions.h"
//
class Client{
    public:
        // Vars
        std::thread thread;
        // Functions
        void Start(const Config &config, Game &game,Player player[]);
        void Thread(const Config &config,Game &game,Player player[]);
        void Ready(Game &game,Player player[]);
        void Process_Packet(const Config &config,Game &game,Player player[],sf::Packet &packet);
        // Constructor
        //Client();
};
#endif // CURVE_CLIENT
//
