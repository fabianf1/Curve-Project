// Contains config class declaration
#ifndef CURVE_PLAYER
#define CURVE_PLAYER
// SFML Headers
//#include <SFML/Network.hpp>
// Standard Headers
#include <math.h>
// Class Headers
#include "config.h"
//#include "game.h"
#include "pending.h"
#include "enum.h"
#include "powerup.h"
// Define
#define PI 3.14159265359
// Extra
class Game;

//
class Player{
    public:
        // Vars
        float x,y,heading;
        float xOLD,yOLD,hOLD;
        sf::Keyboard::Key keyL,keyR;
        bool enabled; // Not needed anymore
        bool death;
        bool ready;
        int deathframe;
        float gap[2];// Time to gap, and gap time;
        sf::String name;
        sf::Color color;
        sf::VertexArray line; // Stores the line
        sf::CircleShape circle;
        int points;
        bool keyrelease;
        //
        int linewidth;
        int sine_linewidth; // Linewidth without the sine;
        float shift;
        float sine_shift; // Speed without the sine;
        float turn;
        bool rightangle;
        bool invisible;
        bool inverted;
        bool noline;
        bool gapping;
        bool sine;
        float sine_phase;
        //
        unsigned int id; // Place in client vector;
        int place; // Place in player array
        bool local;
        bool server;
        bool left,right;
        bool sendlast;
        bool disconnected;
        // Constructor
        Player();
        Player(const sf::String &Name,const sf::Color &Color);
        // Functions
        void New_Game(const Config &config,Game &game);
        void New_Round(const Config &config,Game &game);
        void Draw(sf::RenderWindow &window);
        void Update_Position(const Config &config, Game &game);
        void Update_Position(const Config &config, sf::Packet &packet);
        void Add_Line(const float &X1,const float &X2,const float &Y1,const float &Y2,const float &H1, const float &H2,const int &linewidth);
        void Calculate_Gap(const Config &config);
        void Calculate_Powerup_Effect(const Config &config,const Game &game);
};
//
#include "game.h"
//
#endif // CURVE_PLAYER
