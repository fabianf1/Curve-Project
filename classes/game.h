// Contains game class declaration
#ifndef CURVE_GAME
#define CURVE_GAME
// Standard Headers
#include <vector>
#include <thread>
#include <mutex>
#include <time.h>
#include <iostream>
#include <random>
#include "Powerup.h"
#include "PowerupField.h"
#include "PowerupEffect.h"
#include "Pending.h"
#include "Player.h"
#include "Pacer.h"
#include "Packet.h"
//
class RendererObjects;
//
class Game{
    public:
        // thread Var
        std::thread gameThread; // Stores thread
        //
        int frame; // Keeps track of current frame
        //unsigned int game_frame;
        enum class Mode : int {mainMenu,setup,Play} mode; // Keeps track of current modus
        int keyChange[2]; // Keeps track of keychanges, 1 is player, 0 is key(left=0 or right=1)
        int nameChange;
        int removedPlayer;
        int maxPoints;
        bool powerupEnabled;
        bool countdownEnabled;
        bool multiplePlayersEnabled;
        bool paused;
        unsigned int round;
        unsigned int deathCount;
        bool roundFinished;
        bool gameFinished;
        bool endMessageSet;
        int roundWinner;
        int countdownInt;
        sf::Clock countdownClock;
        std::recursive_mutex modeMutex;
        //
        bool refreshPlayers;
        bool refreshOptions;
        //
        bool wallsAway;
        float wallsAwayTimer;
        bool darkness;
        float darknessTimer;
        int morepowerups;
        float elapsed;
        float powerupSpawnTime;
        //
        int totalChance;
        std::vector<Powerup> powerups;
        std::vector<PowerupField> powerupField; // Keeps track of all powerups on the field
        std::vector<PowerupEffect> powerupEffect; // Stores powerupeffects that effect whole game;
        std::vector<PowerupEffect> playerPowerupEffect;
        //
        bool updateThread[3];// 0=Cleanup required,1=running,2=shutdown;
        bool server[3];
        bool client[3];
        unsigned int id;
        sf::IpAddress serverIp;
        int packetNumber;
        float packetTime;
        //
        std::vector<Pending> packets;
        sf::Mutex packetMutex;
        // Clocks
        sf::Clock fpsClock; // For calculating fps
        sf::Clock game_clock; // For game update
        sf::Clock packetclock; // Measuring time between packets;
        //
        Pacer gamePacer;
        // Random
        std::default_random_engine randGenerator;
        std::uniform_int_distribution<int> randPowerup;
        std::uniform_int_distribution<int> randSpawn;
        // Constructor
        Game(const Config &config);
        // Functions
        void switchMode(const Game::Mode &Mode);
        void initialize(const Config &config,std::vector<Player> &player);
        void initializePowerups(const Config &config);
        void thread(const Config &config,std::vector<Player> &player);
        void newRound(const Config &config,std::vector<Player> &player);
        void hitDetector(const Config &config,std::vector<Player> &player);
        void playerDeath(std::vector<Player> &player,const std::vector<unsigned int> &death_vec);
        void endRound(const Config &config,std::vector<Player> &player);
        void quit(const Config &config);
        void powerUpManager(const Config &config,std::vector<Player> &player);
        void powerUpManager(const Config &config);
        void choosePowerUp(Powerup::Type &type, Powerup::Impact &impact, int &place);
        void powerUpBomb(const Config &config,std::vector<Player> &player, const int &i, const unsigned int &bombNumber);
        void pause(const bool &pause);
        void optionsChanged(RendererObjects &objects);
        void queuePacket(Pending &packet);
        void shutdown();
};
//
#include "rendererObjects.h"
//
#endif // CURVE_GAME
