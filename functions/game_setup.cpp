// Contains game_setup Functions
#include "../curve.h"
//
void Initialize_Game_Setup(const Config &config,Game &game,Player player[]){
    for(int i=0;i<MAX_PLAYERS;i++){
        // Set Player Names
        player[i].name="Player " + int2string(i+1);
        // Set Colors
        player[i].color=config.linecolors[i];
        // Set keys to undefines
        player[i].keyL=player[i].keyR=sf::Keyboard::Unknown;
        // Set to disabled
        player[i].enabled=false;
        // Set Circle
        //player[i].circle.setRadius(config.linewidth/2);
        player[i].circle.setPointCount(50); // Number of sides, more means nicer circle
        player[i].circle.setFillColor(config.linecolors[i]);
    }
    game.mode=Game::Mode::Setup_SP;
}
//
void Initialize_Game_Setup_MP(const Config &config,Game &game,Player player[]){
    for(int i=0;i<MAX_PLAYERS;i++){
        // Set Player Names
        player[i].name="Player " + int2string(i+1);
        // Set Colors
        player[i].color=config.linecolors[i];
        // Set keys to undefines
        player[i].keyL=player[i].keyR=sf::Keyboard::Unknown;
        // Set to disabled
        player[i].enabled=false;
        player[i].ready=false;
        // Set Circle
        //player[i].circle.setRadius(config.linewidth/2);
        player[i].circle.setPointCount(50); // Number of sides, more means nicer circle
        player[i].circle.setFillColor(config.linecolors[i]);
        //
        player[i].Calculate_Powerup_Effect(config,game);
    }
    //
    game.keychange[1]=-1;
    game.online=true;

    game.mode=Game::Mode::Setup_MP;
}
//
void Switch_Player(Game &game,Player player[],int i){
    if(player[i].enabled){
        player[i].enabled=false;
        player[i].keyL=player[i].keyR=sf::Keyboard::Unknown;
        game.keychange[0]=-1;
        game.player_switched=false;
    }
    else{
        player[i].enabled=true;
        game.keychange[0]=i;
        game.keychange[1]=1;
        game.player_switched=true;
    }
}
//
bool Key_Available(Player player[],sf::Keyboard::Key key){
    // Space and escape are not allowed!
    if(key==sf::Keyboard::Escape||key==sf::Keyboard::Space){
        return false;
    }
    // Check if a player already has that key
    for(int i=0;i<MAX_PLAYERS;i++){
        if(!player[i].enabled){continue;}
        if(player[i].keyL==key||player[i].keyR==key){
            return false;
        }
    }
    return true;
}
//
void Auto_Add_Players(Player player[]){
    // Player 1
    player[0].keyL=sf::Keyboard::Left;
    player[0].keyR=sf::Keyboard::Right;
    player[0].enabled=true;
    // Player 2
    player[1].keyL=sf::Keyboard::Z;
    player[1].keyR=sf::Keyboard::X;
    player[1].enabled=true;
}
//
