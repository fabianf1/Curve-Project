// Contains the main Loop
#include "main.h"

//
void Main::curveProject(){
    std::cout << "Starting..." << std::endl;
    // start Renderer
    renderer.start(*this,config,game,storage.objects, player);
    // Main loop
    while(renderer.window.isOpen()){
        // event Handler
        while(renderer.window.pollEvent(event)){
            eventHandler();
        }
        // Process other events
        // The lock is needed so that the fonts remain normal
        if(game.modeMutex.try_lock()){
            // Check if correct mode is enabled; I do not like this solution though
            if(game.mode!=storage.mode){
                if(game.mode==Game::Mode::mainMenu){
                    storage.initMainMenu(config);
                }
                else if(game.mode==Game::Mode::setup){
                    storage.initGameSetup(config);
                }
                else{
                    storage.initGame(config,game,player);
                }
            }
            // Loop Time
            for(unsigned int i=0; i<storage.objects.size();i++){
                if(storage.objects[i]->checkVisible(*this)){
                    storage.objects[i]->process(*this);
                }
            }
            // Unlock
            game.modeMutex.unlock();
        }
        // Delay function
        threadPacer.pace();
    }
    // shutdown procedure
    game.shutdown();
    server.shutdown(game);
    client.shutdown(game);
    renderer.shutdown();
    std::cout << "All Threads Finished!" << std::endl;
}
//
void Main::eventHandler(){
    // General
    // Closes window if close button is presed;
    if(event.type == sf::Event::Closed){
        renderer.window.close();
    }
    // Lost Focus Handler
    else if(event.type == sf::Event::LostFocus&&!game.client[1]&&!game.server[1]){
        game.pause(true);
    }
    else if(game.mode==Game::Mode::mainMenu){
        if(game.joinGame){
            if(event.type==sf::Event::TextEntered && ( (event.text.unicode>=48&&event.text.unicode<=57) || event.text.unicode==46 ) && game.serverIp.size()<15){
                game.serverIp+=static_cast<char>(event.text.unicode);
            }
            else if(event.type==sf::Event::KeyPressed&&event.key.code == sf::Keyboard::BackSpace&&game.serverIp.size()>0){
                //player[game.nameChange].nameChange.erase(player[game.nameChange].nameChange.getSize()-1,1);
                game.serverIp.erase(game.serverIp.size()-1,1);
            }
        }
    }
    // setup screen things
    else if(game.mode==Game::Mode::setup){
        // Key changer
        if(game.keyChange[0]!=-1){
            if(event.type==sf::Event::KeyPressed){
                if(event.key.code==sf::Keyboard::Escape){
                   game.keyChange[0]=-1;
                }
                else if(game.keyChange[0]==0){
                    // check availability
                    if(gameSetup.keyAvailable(player,event.key.code)){
                        player[game.keyChange[1]].keyL=event.key.code;
                        game.keyChange[0]=-1;
                    }
                }
                else if(game.keyChange[0]==1){
                    if(gameSetup.keyAvailable(player,event.key.code)){
                        player[game.keyChange[1]].keyR=event.key.code;
                        game.keyChange[0]=-1;
                    }
                }
            }
        }
        // End key-changer
        #ifdef DEBUG
        // Auto adder, only in debug
        if(event.type==sf::Event::KeyPressed&&event.key.code==sf::Keyboard::Space){
            gameSetup.autoAddPlayers(config,game,player);
            //renderer.objects.syncPlayers(config,game,player);
        }
        #endif
        // Name Changer
        else if(game.nameChange>-1&&event.type==sf::Event::KeyPressed&& (event.key.code==sf::Keyboard::Escape) ){
            game.nameChange=-1;
        }
        else if(game.nameChange>-1&&event.type==sf::Event::KeyPressed&& (event.key.code==sf::Keyboard::Return) ){
            if(player[game.nameChange].nameChange.getSize()>0){
                player[game.nameChange].name=player[game.nameChange].nameChange;
                // Send to others
                if(game.server[1]||game.client[1]){
                    Pending pending;
                    pending.packet << Packet::Name << game.nameChange << player[game.nameChange].name;
                    game.queuePacket(pending);
                }
            }
            game.nameChange=-1;
        }
        else if(game.nameChange>-1){
            // Add maxSize at the end of next line to config
            if(event.type==sf::Event::TextEntered&&( (event.text.unicode>=65&&event.text.unicode<=122)|| (event.text.unicode>=48&&event.text.unicode<=57) ) && player[game.nameChange].nameChange.getSize()<8){
                player[game.nameChange].nameChange+=static_cast<char>(event.text.unicode);
            }
            else if(event.type==sf::Event::KeyPressed&&event.key.code == sf::Keyboard::BackSpace&&player[game.nameChange].nameChange.getSize()>0){
                player[game.nameChange].nameChange.erase(player[game.nameChange].nameChange.getSize()-1,1);
            }
        }
    }
    // Play Game
    else if(game.mode==Game::Mode::Play){
        // End game
        if(game.gameFinished&&event.type==sf::Event::KeyPressed&&event.key.code==sf::Keyboard::Space){
            game.quit(config);
            storage.initGameSetup(config);
            if(game.server[1]){
                // Synchronize all information
                server.syncClients(game,player);
                // Then send a return setup package
                Pending pending;
                pending.packet << Packet::ReturnSetup;
                game.queuePacket(pending);
            }
        }
        // New Round
        else if(game.roundFinished&&!game.client[1]&&event.type==sf::Event::KeyPressed&&event.key.code==sf::Keyboard::Space){
            game.newRound(config,player);
        }
        #ifdef DEBUG
        // pause
        else if(!game.client[1]&&event.type==sf::Event::KeyPressed&&event.key.code==sf::Keyboard::Space){
            game.pause(!game.paused);
        }
        #else
        // When not debugging only unpause as server
        else if(game.server[1]&&game.countdownInt==0&&event.type==sf::Event::KeyPressed&&event.key.code==sf::Keyboard::Space){
            game.pause(false);
        }
        else if(!game.server[1]&&!game.client[1]&&game.countdownInt==0&&event.type==sf::Event::KeyPressed&&event.key.code==sf::Keyboard::Space){
            game.pause(!game.paused);
        }
        #endif
    }
}
