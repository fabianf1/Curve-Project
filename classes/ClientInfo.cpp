// Contains functions and constructors for the client_info class
// Needed Header
#include "ClientInfo.h"
// Constructor
ClientInfo::ClientInfo(): socket(new sf::TcpSocket), ready(false), versionCorrect(false){

}
// Functions
bool ClientInfo::checkID(const unsigned int &n){
    for(unsigned int i=0;i<id.size();i++){
        if(n==id[i]){
            return true;
        }
    }
    return false;
}

void ClientInfo::updateID(const unsigned int &n){
    for(unsigned int i=0;i<id.size();i++){
        if(n==id[i]){
            id[i]--;
            return;
        }
    }
}
