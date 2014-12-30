// Contains functions and constructors for the Game Setup class
// Needed Header
#include "game_setup.h"
// Constructor
Game_Setup::Game_Setup(){
    // Color things
    color[0]=sf::Color::Red;
    color[1]=sf::Color::Yellow;
    color[2]=sf::Color::White;
    color[3]=sf::Color::Green;
    color[4]=sf::Color::Magenta;
    color[5]=sf::Color::Blue;
    for(int i=0;i<6;i++){
        color_used[i]=false;
    }
    // Key Change
    key_change[0]=key_change[1]=-1;
}
//
void Game_Setup::Initialize(const Config &config,Game &game,std::vector<Player> &player){
    // Clear all
    /*if(player.size()>0){
        player.clear();
    }*/
    //
    if(player.size()==0){
        color[0]=sf::Color::Red;
        color[1]=sf::Color::Yellow;
        color[2]=sf::Color::White;
        color[3]=sf::Color::Green;
        color[4]=sf::Color::Magenta;
        color[5]=sf::Color::Blue;
        for(int i=0;i<6;i++){
            color_used[i]=false;
        }
    }
    game.Switch_Mode(Game::Mode::Setup);
}
//
void Game_Setup::Add_Player(const Config &config,Game &game,std::vector<Player> &player){
    // Find Available color
    int new_col;
    for(int i=0;i<6;i++){
        if(!color_used[i]){
            new_col=i;
            color_used[i]=true;
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
void Game_Setup::Remove_Player(const Config &config,Game &game,std::vector<Player> &player, const int &i){
    // Backup color
    sf::Color col=player[i].color;
    // Remove player
    player.erase(player.begin()+i);
    // Set Color Available
    for(int i=0;i<6;i++){
        if(col==color[i]){
            color_used[i]=false;
            break;
        }
    }
    // Done
    std::cout << "Number of players: " << player.size() << std::endl;
}
//
bool Game_Setup::Key_Available(const std::vector<Player> &player,const sf::Keyboard::Key &key){
    // Space and escape are not allowed!
    if(key==sf::Keyboard::Escape||key==sf::Keyboard::Space){
        return false;
    }
    // Check if a player already has that key
    for(unsigned int i=0;i<player.size();i++){
        if(player[i].keyL==key||player[i].keyR==key){
            return false;
        }
    }
    return true;
}
//
void Game_Setup::Join(const Config &config,Game &game,std::vector<Player> &player){
    // Clear all
    /*if(player.size()>0){
        player.clear();
    }*/
    // Try connecting

}
//
void Game_Setup::Quit(const Config &config,Game &game,std::vector<Player> &player){
    // Quit Server and Client and clear up
    //
    game.Switch_Mode(Game::Mode::Main_Menu);
}
//
void Game_Setup::Start_Game(const Config &config,Game &game,std::vector<Player> &player){
    // Check if there are at least two players
    if(player.size()<2){
            std::cout << "Not enough players!" << std::endl;
        return;
    }
    // Check if buttons are set
    for(unsigned int i=0;i<player.size();i++){
        if( (player[i].local&&(player[i].keyL==sf::Keyboard::Unknown||player[i].keyR==sf::Keyboard::Unknown)) || (!player[i].local&&!player[i].ready) ){
            std::cout << "Keys not set or someone is not ready!" << std::endl;
            return;
        }
    }
    // If they are start init
    game.Initialize(config,player);
}
//
void Game_Setup::Auto_Add_Players(const Config &config,Game &game,std::vector<Player> &player){
    if(player.size()>0){
        return;
    }
    Add_Player(config,game,player);
    player[0].keyL=sf::Keyboard::Left;
    player[0].keyR=sf::Keyboard::Right;
    Add_Player(config,game,player);
    player[1].keyL=sf::Keyboard::Z;
    player[1].keyR=sf::Keyboard::X;
}

//
