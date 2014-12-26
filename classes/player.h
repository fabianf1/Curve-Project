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
        int shift;
        int turn;
        bool rightangle;
        bool invisible;
        bool inverted;
        bool noline;
        bool wall;
        //
        unsigned int id; // Place in client vector;
        int place; // Place in player array
        bool local;
        bool server;
        bool left,right;
        bool sendlast;
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
        /*void New_Round(const Config &config,const Game &game);
        void New_Round_Server(const Config &config,Game &game,int i);
        void New_Round_Client(const Config &config,Game &game,sf::Packet &packet);
        void Update_Position(const Config &config, Game &game);
        void Update_Position_Local(const Config &config, Game &game);
        //void Update_Position_Server(const Config &config, Game &game,int i);
        void Update_Position_Client(const Config &config, sf::Packet &packet);
        //void Update_Position_Client2(const Config &config, sf::Packet &packet); // Handles the bigger update package
        void Add_Line(const float &X1,const float &X2,const float &Y1,const float &Y2,const float &H1,const float &H2,const int &linewidth);
        void Calculate_Powerup_Effect(const Config &config,const Game &game);
        void Calculate_Gap(const Config &config);
        void Process_SYNC_Packet(sf::Packet &packet);
        void Name_Packet(Game &game);*/
};
//
#include "game.h"
//
#endif // CURVE_PLAYER
