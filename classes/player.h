// Contains config class declaration
#ifndef CURVE_PLAYER
#define CURVE_PLAYER
// Standard Headers
#include <math.h>
// Class Headers
#include "config.h"
#include "pending.h"
#include "powerup.h"
// Define
#define PI 3.14159265359
// Prototype
class Game;

//
class Player{
    public:
        // Vars
        float x,y,heading;
        float xOLD,yOLD,hOLD;
        sf::Keyboard::Key keyL,keyR;
        bool death;
        bool ready;
        float gap[2];// Time to gap[0], and gap time[1];
        sf::String name;
        sf::Color color;
        sf::VertexArray line; // Stores the line
        sf::CircleShape circle;
        sf::RectangleShape rectangle;
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
        // Constructor
        Player();
        Player(const sf::String &Name,const sf::Color &Color);
        // Functions
        void New_Game();
        void New_Round(const Config &config,const Game &game);
        void Draw(sf::RenderWindow &window);
        void Update_Position(const Config &config, Game &game);
        void Update_Position(const Config &config, sf::Packet &packet,const float &packettime);
        void Add_Line(const float &X1,const float &X2,const float &Y1,const float &Y2,const float &H1, const float &H2,const int &linewidth);
        void Calculate_Gap(const Config &config);
        void Calculate_Powerup_Effect(const Config &config,const Game &game);
};
//
#include "game.h"
//
#endif // CURVE_PLAYER
