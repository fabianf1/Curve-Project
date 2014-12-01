

#include "../curve.h"
//
void Client_Ready(Game &game,Player player[]){
    game.keychange[1]=-1;
    // Check if keys are set
    if(!player[game.id].ready&&player[game.id].keyL!=sf::Keyboard::Unknown&&player[game.id].keyR!=sf::Keyboard::Unknown){
        // Package
        Pending pending;
        pending.packet << Packet::Ready << game.id << true;
        game.mutex.lock();
        game.pending.push_back(pending);
        game.mutex.unlock();
        // Local
        player[game.id].ready=true;
    }
    else if(player[game.id].ready){
        Pending pending;
        pending.packet << Packet::Ready << game.id << false;
        game.mutex.lock();
        game.pending.push_back(pending);
        game.mutex.unlock();
        player[game.id].ready=false;
    }
}
