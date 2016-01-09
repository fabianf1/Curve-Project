// Contains config class declaration
#ifndef CURVE_PLAYER
#define CURVE_PLAYER
// Standard Headers
#include <math.h>
#include "Config.h"
#include "Pending.h"
#include "Powerup.h"
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
        sf::String nameChange;
        sf::Color color;
        sf::VertexArray line; // Stores the line
        sf::CircleShape circle;
        sf::RectangleShape rectangle;
        int points;
        bool keyrelease;
        //
        int lineWidth;
        int originalLineWidth; // Linewidth without the sine;
        float shift;
        float originalShift; // Speed without the sine;
        float turn;
        bool rightAngle;
        bool invisible;
        bool inverted;
        bool noLine;
        bool gapping;
        bool sine;
        float sinePhase;
        bool glitch;
        float glitchTimer;
        float glitchWidthScale;
        float glitchSpeedScale;
        bool radius;
        float noTurtleTimer;
        sf::VertexArray noTurtleLine;
        unsigned int multiplier;
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
        void newRound(const Config &config,const Game &game);
        void updatePosition(const Config &config, Game &game);
        void updatePosition(const Config &config, sf::Packet &packet);
        void addLine(const float &X1,const float &X2,const float &Y1,const float &Y2,const float &H1, const float &H2,const int &lineWidth);
        void calculateGap(const Config &config);
        void calculatePowerupEffect(const Config &config,const Game &game);
        void calculateNoTurtleEffect(const Config &config, const int &i, const Powerup::Impact &Impact);
        void finalizeTurtle(Game &game);
};
//
#include "Game.h"
//
#endif // CURVE_PLAYER
