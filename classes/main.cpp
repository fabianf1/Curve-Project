// Contains the main Loop
#include "main.h"

//
void Main::curveProject(){
    // start Renderer
    renderer.start(config,game,player);
    // Main loop
    while(renderer.window.isOpen()){
        // event Handler
        while(renderer.window.pollEvent(event)){
            eventHandler();
        }
        // Process other events
        // The lock is needed so that the fonts remain normal
        if(game.modeMutex.try_lock()){
            if(game.mode==Game::Mode::mainMenu){
                mainMenuHandler();
            }
            else if(game.mode==Game::Mode::setup){
                gameSetupHandler();
            }
            else if(game.mode==Game::Mode::Play){
                gameHandler();
            }
            // check if player change
            if(renderer.objects.vectorLength<player.size()||renderer.objects.vectorLength>player.size()||game.refreshPlayers){
                renderer.objects.syncPlayers(config,player);
                game.refreshPlayers=false;
            }
            // Options changed
            if(game.refreshOptions){
                game.optionsChanged(renderer.objects);
                game.refreshOptions=false;
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
    // Client connect
    else if(game.mode==Game::Mode::mainMenu){
        if(event.type == sf::Event::KeyPressed && renderer.objects.m_ipPrompt.isEnabled() && event.key.code == sf::Keyboard::Return){
            game.serverIp=sf::IpAddress(renderer.objects.m_ipPrompt.getString());
            client.start(config,game,player);
        }
        else{
            renderer.objects.m_ipPrompt.event(event);
        }
    }
    // setup screen things
    else if(game.mode==Game::Mode::setup){
        // Key changer
        if(gameSetup.keyChange[0]!=-1){
            if(event.type==sf::Event::KeyPressed){
                if(event.key.code==sf::Keyboard::Escape){
                    changeButton(-1,0);
                }
                else if(gameSetup.keyChange[0]==0){
                    // check availability
                    if(gameSetup.keyAvailable(player,event.key.code)){
                        player[gameSetup.keyChange[1]].keyL=event.key.code;
                        changeButton(-1,0);
                    }
                }
                else if(gameSetup.keyChange[0]==1){
                    if(gameSetup.keyAvailable(player,event.key.code)){
                        player[gameSetup.keyChange[1]].keyR=event.key.code;
                        changeButton(-1,0);
                    }
                }
                // Update renderer.objects
                renderer.objects.syncPlayers(config,player);
            }
        }
        // End key-changer
        #ifdef DEBUG
        // Auto adder, only in debug
        else if(event.type==sf::Event::KeyPressed&&event.key.code==sf::Keyboard::Space){
            gameSetup.autoAddPlayers(config,game,player);
            renderer.objects.syncPlayers(config,player);
        }
        #endif
        // Name Changer
        else if(game.nameChange>-1&&event.type==sf::Event::KeyPressed&& (event.key.code==sf::Keyboard::Escape) ){
            renderer.objects.s_names[game.nameChange].setActive(false);
            game.nameChange=-1;
            // Reset
            renderer.objects.syncPlayers(config,player);
        }
        else if(game.nameChange>-1&&event.type==sf::Event::KeyPressed&& (event.key.code==sf::Keyboard::Return) ){
            if(renderer.objects.s_names[game.nameChange].getString().getSize()==0){
                renderer.objects.syncPlayers(config,player);
            }
            else{
                player[game.nameChange].name=renderer.objects.s_names[game.nameChange].getString();
                // Send to others
                if(game.server[1]||game.client[1]){
                    Pending pending;
                    pending.packet << Packet::Name << game.nameChange << player[game.nameChange].name;
                    game.queuePacket(pending);
                }
            }
            renderer.objects.s_names[game.nameChange].setActive(false);
            game.nameChange=-1;
        }
        else if(game.nameChange>-1){
            renderer.objects.s_names[game.nameChange].event(event);
        }
    }
    // Play Game
    else if(game.mode==Game::Mode::Play){
        // End game
        if(game.gameFinished&&event.type==sf::Event::KeyPressed&&event.key.code==sf::Keyboard::Space){
            game.quit(config);
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
//
void Main::mainMenuHandler(){
    // Create Button
    if(renderer.objects.m_create.check(renderer.window)){
        if(renderer.objects.m_ipPrompt.isEnabled()){
            renderer.objects.m_ipPrompt.toggle();
        }
        renderer.objects.setOptions(game);
        gameSetup.initialize(config,game,player);
        return;
    }
    // Connect
    if(renderer.objects.m_join.check(renderer.window)){
        // Enable prompt
        renderer.objects.m_ipPrompt.toggle();
    }
    // Connect Confirm button
    if(renderer.objects.m_ipPrompt.check(renderer.window)){
        //
        if(renderer.objects.m_ipPrompt.getString().getSize()>6){
            game.serverIp=sf::IpAddress(renderer.objects.m_ipPrompt.getString());
            client.start(config,game,player);
        }
    }
    // Quit
    if(renderer.objects.m_quit.check(renderer.window)){
        renderer.window.close();
    }
}
//
void Main::gameSetupHandler(){
    // Players
    for(unsigned int i=0;i<player.size()&&i<renderer.objects.vectorLength;i++){
        // Name
        if(renderer.objects.s_names[i].check(renderer.window)){
            // Name Change
            if(player[i].local && (!game.client[1] || (game.client[1]&&!client.ready) ) ){
                if(game.nameChange==i){
                    if(renderer.objects.s_names[i].getString().getSize()==0){
                        renderer.objects.syncPlayers(config,player);
                    }
                    else{
                        player[i].name=renderer.objects.s_names[i].getString();
                        if(game.server[1]||game.client[1]){
                            Pending pending;
                            pending.packet << Packet::Name << i << player[i].name;
                            game.queuePacket(pending);
                        }
                    }
                    renderer.objects.s_names[i].setActive(false);
                    game.nameChange=-1;
                }
                else if(game.nameChange>-1){
                    renderer.objects.s_names[game.nameChange].setActive(false);
                    game.nameChange=i;
                    renderer.objects.s_names[i].setActive(true);
                    changeButton(-1,0);
                }
                else{
                    game.nameChange=i;
                    renderer.objects.s_names[i].setActive(true);
                    changeButton(-1,0);
                }
            }
        }
        // Left/Right
        if(player[i].local){
            // Buttons
            if(renderer.objects.s_leftButton[i].check(renderer.window) && (!game.client[1] || (game.client[1]&&!client.ready) )){
                changeButton(0,i);
                if(game.nameChange>-1){
                    renderer.objects.s_names[i].setActive(false);
                    game.nameChange=-1;
                }
            }
            if(renderer.objects.s_rightButton[i].check(renderer.window) && (!game.client[1] || (game.client[1]&&!client.ready) )){
                changeButton(1,i);
                if(game.nameChange>-1){
                    renderer.objects.s_names[i].setActive(false);
                    game.nameChange=-1;
                }
            }
        }
        // Status
        if(player[i].server){
            renderer.objects.s_status[i].setString("Server");
        }
        // Server side
        else if(!player[i].server&&!player[i].local
                && (game.server[1] && !server.clients[player[i].id].ready) ){
            renderer.objects.s_status[i].setString("Not ready: " + int2str(player[i].id) );
        }
        else if(!player[i].server&&!player[i].local
                && (game.server[1] && server.clients[player[i].id].ready) ){
            renderer.objects.s_status[i].setString("Ready: " + int2str(player[i].id) );
        }
        // Clients
        else if(!player[i].server&&!player[i].local
                && (game.client[1] && !player[i].ready ) ){
            renderer.objects.s_status[i].setString("Not ready");
        }
        else if(!player[i].server&&!player[i].local
                && (game.client[1] && player[i].ready ) ){
            renderer.objects.s_status[i].setString("Ready");
        }
        // Kick
        // Local: Can kick all. Server: Can kick all but first player. Client: Can kick local players except first
        if( ( (game.client[1] && i!=game.id && player[i].local) || (game.server[1] && i!=0 ) || (!game.server[1]&&!game.client[1] ) ) && renderer.objects.s_kick[i].check(renderer.window)){
            // Remove from server
            if(!game.client[1]){
                if(game.server[1]){
                    if(!player[i].local){
                        // Remove from clients
                        for(unsigned int j=0;j<server.clients[player[i].id].id.size();j++){
                            if(server.clients[player[i].id].id[j]==i){
                                server.clients[player[i].id].id.erase(server.clients[player[i].id].id.begin()+j);
                                break;
                            }
                        }
                        // Remove if no player left
                        if(server.clients[player[i].id].id.size()==0){
                            server.clientMutex.lock();
                            server.selector.remove(*server.clients[player[i].id].socket);
                            server.clients.erase(server.clients.begin()+player[i].id);
                            server.clientMutex.unlock();
                        }
                        // Update player id's
                        gameSetup.removePlayer(game,player,i);
                        server.updatePlayerID(player,i);
                    }
                    else{
                        gameSetup.removePlayer(game,player,i);
                    }
                    //
                    Pending pending;
                    pending.packet << Packet::Disconnect << i;
                    game.queuePacket(pending);
                }
                else{
                    gameSetup.removePlayer(game,player,i);
                }
                //
                renderer.objects.syncPlayers(config,player);
            }
            else{
                player[i].local=false;
                Pending pending;
                pending.packet << Packet::RemovePlayer << i;
                game.queuePacket(pending);
            }
            break;
        }
    }
    // Options
    if(renderer.objects.s_max10.check(renderer.window)&&!game.client[1]){
        game.maxPoints=10;
        game.optionsChanged(renderer.objects);
    }
    if(renderer.objects.s_max20.check(renderer.window)&&!game.client[1]){
        game.maxPoints=20;
        game.optionsChanged(renderer.objects);
    }
    if(renderer.objects.s_max40.check(renderer.window)&&!game.client[1]){
        game.maxPoints=40;
        game.optionsChanged(renderer.objects);
    }
    if(renderer.objects.s_powerupOff.check(renderer.window)&&!game.client[1]){
        game.powerupEnabled=false;
        game.optionsChanged(renderer.objects);
    }
    if(renderer.objects.s_powerupOn.check(renderer.window)&&!game.client[1]){
        game.powerupEnabled=true;
        game.optionsChanged(renderer.objects);
    }
    if(!game.client[1]&&!game.server[1]){
        if(renderer.objects.s_countdownOff.check(renderer.window)){
            game.countdownEnabled=false;
            game.optionsChanged(renderer.objects);
        }
        if(renderer.objects.s_countdownOn.check(renderer.window)){
            game.countdownEnabled=true;
            game.optionsChanged(renderer.objects);
        }
    }
    else{
        if(renderer.objects.s_multiplePlayersOff.check(renderer.window)&&!game.client[1]){
            game.multiplePlayersEnabled=false;
            game.optionsChanged(renderer.objects);
        }
        if(renderer.objects.s_multiplePlayersOn.check(renderer.window)&&!game.client[1]){
            game.multiplePlayersEnabled=true;
            game.optionsChanged(renderer.objects);
        }
    }
    // Buttons
    if(player.size()<config.maxPlayers&& ( (game.client[1]&&!client.ready&&game.multiplePlayersEnabled) || !game.client[1] ) && renderer.objects.s_add.check(renderer.window)){
        // Send request
        if(game.client[1]){
            Pending pending;
            pending.packet << Packet::RequestPlayer;
            game.queuePacket(pending);
        }
        else{
            // Add new player
            gameSetup.addPlayer(game,player);
            renderer.objects.syncPlayers(config,player);
            // Send to clients
            if(game.server[1]){
               server.syncClients(game,player);
            }
        }
    }
    // Server Text
    if(game.server[1]){
        renderer.objects.s_server.setString("Local: " + sf::IpAddress::getLocalAddress().toString());
    }
    else if(game.client[1]){
        renderer.objects.s_server.setString("Server: " + game.serverIp.toString());
    }
    else if(!game.server[1]&&!game.client[1]){
        renderer.objects.s_server.setString("Start Server");
    }
    // Server Button
    if(renderer.objects.s_server.check(renderer.window)){
        // Depending on situation: start Server, Stop Server or do nothing.
        if(game.client[1]){
            //
        }
        else if(!game.server[1]){
            // start Server
            server.start(config,gameSetup,game,player);
        }
        else{
            server.shutdown(game);
        }
    }
    // start Button
    if(renderer.objects.s_start.check(renderer.window)){
        if(!game.client[1]){
            // Otherwise check if all players are ready and start
            gameSetup.startGame(config,game,player,server);
        }
        else{
            changeButton(-1,0);
            if(game.nameChange>-1){
                renderer.objects.s_names[game.nameChange].setActive(false);
                game.nameChange=-1;
                renderer.objects.syncPlayers(config,player);
            }
            // Client Things
            client.toggleReady(game,player);
        }
    }
    if( (game.client[1] && !client.ready) && renderer.objects.s_start.getString()!="Ready" ){
        renderer.objects.s_start.setString("Ready!");
        renderer.objects.s_start.setPosition(config.windowWidth/2-renderer.objects.s_start.getLocalBounds().width/2,config.windowHeight-100);
    }
    else if(game.client[1]&&client.ready&&renderer.objects.s_start.getString()!="Waiting"){
        renderer.objects.s_start.setString("Waiting!");
        renderer.objects.s_start.setPosition(config.windowWidth/2-renderer.objects.s_start.getLocalBounds().width/2,config.windowHeight-100);
    }
    else if(renderer.objects.s_start.getString()!="Start Game"){
        renderer.objects.s_start.setString("start Game");
        renderer.objects.s_start.setPosition(config.windowWidth/2-renderer.objects.s_start.getLocalBounds().width/2,config.windowHeight-100);
    }
//
    if(renderer.objects.s_quit.check(renderer.window)){
        // To main menu
        gameSetup.quit(config,game,player);
    }
}
//
void Main::changeButton(const int &button,unsigned const int &pl){
    // Set game variables
    gameSetup.keyChange[0]=button;
    gameSetup.keyChange[1]=pl;
    // Set graphics
    for(unsigned int i=0;i<player.size()&&i<renderer.objects.vectorLength;i++){
        if(button!=-1&&pl==i){
            if(button==0){
                renderer.objects.s_leftButton[i].setActive(true);
                renderer.objects.s_rightButton[i].setActive(false);
            }
            else{
                renderer.objects.s_leftButton[i].setActive(false);
                renderer.objects.s_rightButton[i].setActive(true);
            }
        }
        else{
            renderer.objects.s_leftButton[i].setActive(false);
            renderer.objects.s_rightButton[i].setActive(false);
        }
    }
}
//
void Main::gameHandler(){
    if(renderer.objects.g_quit.check(renderer.window)){
        game.quit(config);
    }
    // Change render objects
    for(unsigned int i=0;i<player.size()&&i<renderer.objects.vectorLength;i++){
        renderer.objects.g_scores[i].setString(int2str(player[i].points));
    }
    // Update round number
    if(renderer.objects.g_round[1].getString()!=int2str(game.round)){
        renderer.objects.g_round[1].setString(int2str(game.round));
    }
    // Countdown timer
    if(game.countdownInt>0){
        renderer.objects.g_countdown.setString(int2str(game.countdownInt));
        renderer.objects.g_countdown.setPosition(config.windowWidth/2-config.statusWidth/2-renderer.objects.g_countdown.getLocalBounds().width/2,config.windowHeight/2-renderer.objects.g_countdown.getLocalBounds().height/2);
        if(game.countdownInt==3){
            renderer.objects.g_countdown.setColor(sf::Color::Red);
        }
        else if(game.countdownInt==2){
            renderer.objects.g_countdown.setColor(sf::Color::Yellow);
        }
        else{
            renderer.objects.g_countdown.setColor(sf::Color::Green);
        }
    }
    //
    if(game.roundFinished&&!game.endMessageSet){
        if(game.gameFinished){
            renderer.objects.g_endRoundMessage.setString(player[game.roundWinner].name + " has won the game!");
            renderer.objects.g_endRoundMessage.setColor(player[game.roundWinner].color);
        }
        else if(game.roundWinner==-1){
            renderer.objects.g_endRoundMessage.setString("Round finished!");
            renderer.objects.g_endRoundMessage.setColor(sf::Color::Red);
        }
        else{
            renderer.objects.g_endRoundMessage.setString(player[game.roundWinner].name + " has won the round!");
            renderer.objects.g_endRoundMessage.setColor(player[game.roundWinner].color);
        }
        renderer.objects.g_endRoundMessage.setPosition(config.windowWidth/2-config.statusWidth/2-renderer.objects.g_endRoundMessage.getLocalBounds().width/2,config.windowHeight/2-renderer.objects.g_endRoundMessage.getLocalBounds().height/2);
        game.endMessageSet=true;
    }
}
//
