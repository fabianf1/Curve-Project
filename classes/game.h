// Contains game class declaration
#ifndef CURVE_GAME
#define CURVE_GAME
// Standard Headers
#include <vector>
#include <thread>
#include <mutex>
#include <time.h>
#include <iostream>
// Class Headers
#include "powerup.h"
#include "powerup_effect.h"
#include "pending.h"
#include "player.h"
// Temporary
void Sleep(const int &frame_time,sf::Clock &clock);
//
class Game{
    public:
        // Thread Var
        std::thread thread; // Stores Thread
        //
        int frame; // Keeps track of current frame
        //unsigned int game_frame;
        enum class Mode : int {Main_Menu,Setup,Play} mode; // Keeps track of current modus
        int keychange[2]; // Keeps track of keychanges, 1 is player, 0 is key(left=0 or right=1)
        int name_change;
        int maxpoints;
        bool powerup_enabled;
        bool player_switched;
        bool pause;
        int players;
        unsigned int round;
        unsigned int deathcount;
        bool round_finished;
        bool game_finished;
        bool end_message_set;
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
        bool connected;
        unsigned int id;
        sf::IpAddress server_ip;
        int packetnumber;
        float packettime;
        //
        std::vector<Pending> packets;
        sf::Mutex mutex;
        //
        //std::mutex mode_lock;
        // Clocks
        sf::Clock randclock; //.For initializing the rand();
        sf::Clock fps_clock; // For calculating fps
        sf::Clock game_clock; // For game update
        sf::Clock game_update_thread_clock; // For limiting game_update thread
        sf::Clock main_thread_clock; // For limiting main thread
        sf::Clock packetclock; // Measuring time between packets;
        // Constructor
        Game();
        // Functions
        void Switch_Mode(const Game::Mode &Mode);
        void Initialize(const Config &config,Game &game,std::vector<Player> &player);
        void Thread(const Config &config,Game &game,std::vector<Player> &player);
        void New_Round(const Config &config,Game &game,std::vector<Player> &player);
        void Hit_Detector(const Config &config,std::vector<Player> &player);
        void Player_Death(std::vector<Player> &player,std::vector<unsigned int> &death_vec);
        void Add_Points(std::vector<Player> &player, std::vector<unsigned int> &death_vec);
        void End_Round(const Config &config,std::vector<Player> &player);
        void Quit(const Config &config,std::vector<Player> &player);
        void PowerUp_Manager(const Config &config,Game &game,std::vector<Player> &player);
        void PowerUp_Manager(const Config &config,Game &game);
        void Pause(const Config &config, const bool &Pause);
        void Shutdown();
};
#endif // CURVE_GAME
