// Contains functions and constructors for the config class
// Needed Header
#include "..\curve.h"
// Constructor
Game::Game(){
    frame=0;
    lastclick=0;
    keychange[0]=-1;
    name_change=false;
    maxpoints=10;
    mode=Mode::Main_Menu;
    powerup_enabled=true;
    player_switched=false;
    morepowerups=0;
    update_thread[0]=update_thread[1]=update_thread[2]=false;
    server[0]=server[1]=server[2]=false;
    client[0]=client[1]=client[2]=false;
    online=false;
    connected=false;
    server_ip=sf::IpAddress::getLocalAddress().toString();
    countdown_int=0;
}
