// Contains functions and constructors for the Game setup class
// Needed Header
#include "GameSetup.h"
// Constructor
GameSetup::GameSetup(){
    // Color things
    color[0]=sf::Color::Red;
    color[1]=sf::Color::Yellow;
    color[2]=sf::Color::White;
    color[3]=sf::Color::Green;
    color[4]=sf::Color::Magenta;
    color[5]=sf::Color::Blue;
    color[6]=sf::Color::Cyan;
    color[7]=sf::Color(255,165,0);
    for(int i=0;i<8;i++){
        colorUsed[i]=false;
    }
}
//
void GameSetup::initialize(const Config &config,Game &game,std::vector<Player> &player){
    //
    if(player.size()==0){
        for(int i=0;i<6;i++){
            colorUsed[i]=false;
        }
    }
    game.switchMode(Game::Mode::setup);
}
//
void GameSetup::addPlayer(Game &game,std::vector<Player> &player){
    // Find Available color
    int new_col;
    for(int i=0;i<8;i++){
        if(!colorUsed[i]){
            new_col=i;
            colorUsed[i]=true;
            break;
        }
    }
    sf::String str="Player";
    player.emplace_back(str,color[new_col]);
    // Server things
    player.back().server=true;
    player.back().local=true;
    player.back().ready=false;
    // Yeeh!
    std::cout << "Number of players: " << player.size() << std::endl;
}
//
void GameSetup::removePlayer(Game &game,std::vector<Player> &player, const int &i){
    // Backup color
    sf::Color col=player[i].color;
    // Remove player
    player.erase(player.begin()+i);
    // Set Color Available
    for(int i=0;i<6;i++){
        if(col==color[i]){
            colorUsed[i]=false;
            break;
        }
    }
    // Done
    std::cout << "Number of players: " << player.size() << std::endl;
}
//
bool GameSetup::keyAvailable(const std::vector<Player> &player,const sf::Keyboard::Key &key){
    // Space and escape are not allowed!
    if(key==sf::Keyboard::Escape||key==sf::Keyboard::Space){
        return false;
    }
    // check if a player already has that key
    for(unsigned int i=0;i<player.size();i++){
        if(player[i].keyL==key||player[i].keyR==key){
            return false;
        }
    }
    return true;
}
//
void GameSetup::quit(const Config &config,Game &game,std::vector<Player> &player){
    // Quit Server and Client and clear up
    if(game.client[1]){
        game.client[2]=true;
    }
    else if(game.server[1]){
        game.server[2]=true;
    }
    //
    game.switchMode(Game::Mode::mainMenu);
}
//
void GameSetup::startGame(const Config &config,Game &game,std::vector<Player> &player,const Server &server){
    // check if there are at least two players
    if(player.size()<2){
        std::cout << "Not enough players!" << std::endl;
        return;
    }
    // check keys
    for(unsigned int i=0;i<player.size();i++){
        if( (player[i].local&&(player[i].keyL==sf::Keyboard::Unknown||player[i].keyR==sf::Keyboard::Unknown))){
            std::cout << "Keys not set!" << std::endl;
            return;
        }
    }
    // check ready
    for(unsigned int i=0;i<server.clients.size();i++){
        if(!server.clients[i].ready){
            std::cout << "Someone is not ready!" << std::endl;
            return;
        }
    }
    // shutdown server if no clients
    if(server.clients.size()==0&&game.server[1]){
        game.server[2]=true;
    }
    // If they are start init
    game.initialize(config,player);
}
//
void GameSetup::autoAddPlayers(const Config &config,Game &game,std::vector<Player> &player){
    if(player.size()>0){
        return;
    }
    addPlayer(game,player);
    player[0].keyL=sf::Keyboard::Left;
    player[0].keyR=sf::Keyboard::Right;
    addPlayer(game,player);
    player[1].keyL=sf::Keyboard::Z;
    player[1].keyR=sf::Keyboard::X;
}
//
