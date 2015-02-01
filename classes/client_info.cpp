// Contains functions and constructors for the client_info class
// Needed Header
#include "client_info.h"
// Constructor
Client_Info::Client_Info(): socket(new sf::TcpSocket), ready(false){}
// Functions
bool Client_Info::Check_ID(const unsigned int &n){
    for(unsigned int i=0;i<id.size();i++){
        if(n==id[i]){
            return true;
        }
    }
    return false;
}
