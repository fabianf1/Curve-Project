// Contains game class declaration
#ifndef CURVE_GAME
#define CURVE_GAME
// Standard Headers
#include <vector>
#include <thread>
// Class Headers
#include "powerup.h"
#include "powerup_effect.h"
#include "pending.h"
//
class Game{
    public:
        // Thread Vars
        std::vector<std::thread> threads; // Stores Threads
        //
        int frame; // Keeps track of current frame
        //unsigned int game_frame;
        int lastclick; // Keeps track of lastclick
        enum class Mode : int {Main_Menu,Setup_SP,Setup_MP,Server_IP,Play,Play_MP} mode; // Keeps track of current modus
        int keychange[2]; // Keeps track of keychanges, 0 is player, 1 is key(left or right)
        bool name_change;
        int maxpoints;
        bool powerup_enabled;
        bool player_switched;
        bool pause;
        int players;
        int round;
        int deathcount;
        bool round_finished;
        bool game_finished;
        int round_winner;
        int last_fps;
        int countdown_int;
        sf::Clock countdown;
        //
        bool wallsaway;
        int morepowerups;
        float elapsed;
        float powerup_spawn_time;
        //
        std::vector<Powerup> powerup; // Keeps track of all powerups
        std::vector<Powerup_Effect> powerup_effect; // Stores powerupeffects that effect whole game;
        std::vector<Powerup_Effect> player_powerup_effect;
        //
        bool update_thread[3];// 0=start,1=running,2=shutdown;
        bool server[3];// 0=start,1=running,2=shutdown;
        bool client[3];
        bool online;
        bool connected;
        int id;
        sf::String server_ip;
        sf::TcpSocket socket;
        bool send;
        int packetnumber;
        float packettime;
        //
        std::vector<Pending> pending;
        sf::Mutex mutex;
        // Clocks
        sf::Clock randclock; //.For initializing the rand();
        sf::Clock fps_clock; // For calculating fps
        sf::Clock game_clock; // For game update
        sf::Clock game_update_thread_clock; // For limiting game_update thread
        sf::Clock main_thread_clock; // For limiting main thread
        sf::Clock packetclock; // Measuring time between packets;
        // Constructor
        Game();
};
#endif // CURVE_GAME
