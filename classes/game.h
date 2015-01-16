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
#include "powerup_field.h"
#include "powerup_effect.h"
#include "pending.h"
#include "player.h"
#include "pacer.h"
#include "packet.h"
//
class Renderer_Objects;
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
        bool countdown_enabled;
        bool pause;
        unsigned int round;
        unsigned int deathcount;
        bool round_finished;
        bool game_finished;
        bool end_message_set;
        int round_winner;
        int last_fps;
        int countdown_int;
        sf::Clock countdown;
        std::recursive_mutex mode_mutex;
        //
        bool refresh_players;
        bool refresh_options;
        //
        bool wallsaway;
        float wallsaway_timer;
        bool darkness;
        float darkness_timer;
        int morepowerups;
        float elapsed;
        float powerup_spawn_time;
        //
        int total_chance;
        std::vector<Powerup> powerups;
        std::vector<Powerup_Field> powerup_field; // Keeps track of all powerups on the field
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
        sf::Clock fps_clock; // For calculating fps
        sf::Clock game_clock; // For game update
        sf::Clock packetclock; // Measuring time between packets;
        //
        Pacer game_pacer;
        // Constructor
        Game(const Config &config);
        // Functions
        void Switch_Mode(const Game::Mode &Mode);
        void Initialize(const Config &config,std::vector<Player> &player);
        void Thread(const Config &config,std::vector<Player> &player);
        void New_Round(const Config &config,std::vector<Player> &player);
        void Hit_Detector(const Config &config,std::vector<Player> &player);
        void Player_Death(std::vector<Player> &player,const std::vector<unsigned int> &death_vec);
        void End_Round(const Config &config,std::vector<Player> &player);
        void Quit(const Config &config);
        void PowerUp_Manager(const Config &config,std::vector<Player> &player);
        void PowerUp_Manager(const Config &config);
        void Choose_PowerUp(Powerup::Type &type, Powerup::Impact &impact, int &place);
        void PowerUp_Bomb(const Config &config,std::vector<Player> &player, const int &i);
        void Pause(const bool &Pause);
        void Options_Changed(Renderer_Objects &objects);
        void Shutdown();
};
//
#include "renderer_objects.h"
//
#endif // CURVE_GAME
