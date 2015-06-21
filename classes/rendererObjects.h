// Contains Renderer class declaration
#ifndef CURVE_RENDER_OBJECT
#define CURVE_RENDER_OBJECT
// SFML Headers
#include <vector>
#include <Windows.h> // I really should test on linux
#include <iostream>
#include <SFML/Graphics.hpp>
#include "Config.h"
#include "TextButton.h"
#include "TextBox.h"
#include "Prompt.h"
#include "Player.h"
#include "../functions/General.h"
//

//
class RendererObjects{
    public:
        // Variables
        int fontTitleSize;
        int fontSize;
        int g_fontTitleSize;
        int g_fontSize;
        unsigned int vectorLength; // To keep track of vector lengths
        // Font
        const sf::Font font;
        // Main Menu
        sf::Text m_title;
        sf::Text m_version;
        TextButton m_create;
        TextButton m_join;
        TextButton m_quit;
        // Join game things
        Prompt m_ipPrompt;
        // End Main Menu
        // Game setup
        sf::Text s_title;
        sf::Text s_name;
        sf::Text s_left;
        sf::Text s_right;
        // Player Things
        std::vector<TextBox> s_names;
        std::vector<TextButton> s_leftButton;
        std::vector<TextButton> s_rightButton;
        std::vector<sf::Text> s_status;
        std::vector<TextButton> s_kick;
        TextButton s_add;
        TextButton s_server;
        // Options
        sf::Text s_options;
        sf::Text s_maxPoints;
        TextButton s_max10,s_max20,s_max40;
        sf::Text s_powerups;
        TextButton s_powerupOn,s_powerupOff;
        sf::Text s_countdown;
        TextButton s_countdownOn,s_countdownOff;
        sf::Text s_multiplePlayers;
        TextButton s_multiplePlayersOn,s_multiplePlayersOff;
        // Buttons :D
        TextButton s_start;
        TextButton s_quit;
        // End Game setup
        // Game
        sf::RectangleShape g_leftWall,g_topWall,g_rightWall,g_bottomWall;
        sf::Text g_scoreTitle;
        std::vector<sf::Text> g_names;
        std::vector<sf::Text> g_scores;
        sf::Text g_round[2];
        sf::Text g_fps[2];
        sf::Text g_packet[2];
        TextButton g_quit;
        sf::Text g_endRoundMessage;
        sf::Text g_countdown;
        // End Game
        // Constructor
        RendererObjects(const Config &config);
        // Constructor init functions
        void initMainMenu(const Config &config);
        void initGameSetup(const Config &config);
        void initGame(const Config &config);
        // Vector things
        void syncPlayers(const Config &config,Game &game, const std::vector<Player> &player);
        void addPlayer(const Config &config,const std::vector<Player> &player);
        //
        void setOptions(const Game &game);
        // Other Functions
        sf::Font loadFont(const std::string& name);
        const char* getKeyName(const sf::Keyboard::Key &key );
};

#endif // CURVE_RENDER_OBJECT




