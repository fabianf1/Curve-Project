// Contains the event handler
// Headers
#include "../curve.h"
//
void Pause_Game(const Config &config,Game &game,bool pause){
    if(pause){
        game.pause=true;
        if(game.server[1]){
            Pending pending;
            pending.packet << Packet::Pause << true;
            pending.send_id.push_back(-1);
            game.mutex.lock();
            game.pending.push_back(pending);
            game.mutex.unlock();
        }
    }
    else{
        game.pause=false;
        game.game_clock.restart();
        if(game.server[1]){
            game.packetclock.restart();
            Pending pending;
            pending.packet << Packet::Pause << false;
            pending.send_id.push_back(-1);
            game.mutex.lock();
            game.pending.push_back(pending);
            game.mutex.unlock();
        }
        game.countdown_int=0;
    }
}

