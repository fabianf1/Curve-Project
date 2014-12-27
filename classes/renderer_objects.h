// Contains Renderer class declaration
#ifndef CURVE_RENDER_OBJECT
#define CURVE_RENDER_OBJECT
// SFML Headers
#include <SFML/Graphics.hpp>
// Standard headers
#include <vector>
#include <Windows.h> // I really should test on linux
#include <iostream>
// Class Headers
#include "config.h"
#include "text_button.h"
#include "text_box.h"
#include "prompt.h"
#include "player.h"
//
sf::String int2string(int x);

//
class Renderer_Objects{
    public:
        // Variables
        int font_title_size;
        int font_size;
        int g_font_title_size;
        int g_font_size;
        unsigned int vector_length; // To keep track of vector lengths
        // Font
        sf::Font font;
        // Main Menu
        sf::Text m_title;
        Text_Button m_create;
        Text_Button m_join;
        Text_Button m_quit;
        // Join game things
        Prompt m_ip_prompt;
        // End Main Menu
        // Game Setup
        sf::Text s_title;
        sf::Text s_name;
        sf::Text s_left;
        sf::Text s_right;
        // Player Things
        std::vector<Text_Box> s_names;
        std::vector<Text_Button> s_lbutton;
        std::vector<Text_Button> s_rbutton;
        std::vector<sf::Text> s_status;
        std::vector<Text_Button> s_kick;
        Text_Button s_add;
        Text_Button s_server;
        // Options
        sf::Text s_options;
        sf::Text s_maxpoints;
        Text_Button s_max10,s_max20,s_max40;
        sf::Text s_powerups;
        Text_Button s_powerupon,s_powerupoff;
        // Buttons :D
        Text_Button s_start;
        Text_Button s_quit;
        // End Game Setup
        // Game
        sf::RectangleShape g_leftwall,g_topwall,g_rightwall,g_bottomwall;
        sf::Text g_scores;
        std::vector<sf::Text> g_names;
        std::vector<sf::Text> g_score;
        sf::Text g_round[2];
        sf::Text g_frame[2];
        sf::Text g_fps[2];
        sf::Text g_packet[2];
        Text_Button g_quit;
        sf::Text g_end_round_message;
        sf::Text g_countdown;
        // End Game
        // Constructor
        Renderer_Objects(const Config &config);
        // Constructor init functions
        void Init_Main_Menu(const Config &config);
        void Init_Game_Setup(const Config &config);
        void Init_Game(const Config &config);
        // Vector things
        void Sync_Players(const Config &config,Game &game,const std::vector<Player> &player);
        void Add_Player(const Config &config,const std::vector<Player> &player);
        //
        void setOptions(const Game &game);
        // Other Functions
        sf::Font LoadFont(const std::string& name);
        const char* getKeyName( const sf::Keyboard::Key &key );
};

#endif // CURVE_RENDER_OBJECT




