// Contains the main Loop
#include "main.h"

//
void Main::Curve_Project(){
    // Start Renderer
    renderer.Start(config,game,player);
    // Main loop
    while(renderer.window.isOpen()){
        // Event Handler
        while (renderer.window.pollEvent(event)){
            Event_Handler();
        }
        // Process other events
        // The lock is needed so that the fonts remain normal
        if(game.mode_mutex.try_lock()){
            if(game.mode==Game::Mode::Main_Menu){
                Main_Menu_Handler();
            }
            else if(game.mode==Game::Mode::Setup){
                Game_Setup_Handler();
            }
            else if(game.mode==Game::Mode::Play){
                Play_Handler();
            }
            // Check if new player is added
            if(renderer.objects.vector_length<player.size()||renderer.objects.vector_length>player.size()||game.refresh_players){
                renderer.objects.Sync_Players(config,player);
                game.refresh_players=false;
            }
            // Options changed
            if(game.refresh_options){
                game.Options_Changed(renderer.objects);
                game.refresh_options=false;
            }
            // Unlock
            game.mode_mutex.unlock();
        }
        // Delay function
        thread_pacer.Pace();
    }
    // Shutdown procedure
    game.Shutdown();
    server.Shutdown(game);
    client.Shutdown(game);
    renderer.Shutdown();
    std::cout << "All Threads Finished!" << std::endl;
}
//
void Main::Event_Handler(){
    // General
    // Closes window if close button is presed;
    if(event.type == sf::Event::Closed){
        renderer.window.close();
    }
    // Lost Focus Handler
    else if(event.type == sf::Event::LostFocus&&!game.client[1]&&!game.server[1]){
        game.Pause(true);
    }
    // Client connect
    else if(game.mode==Game::Mode::Main_Menu){
        renderer.objects.m_ip_prompt.Event(event);
    }
    // Setup screen things
    else if(game.mode==Game::Mode::Setup){
        // Key changer
        if(game_setup.key_change[0]!=-1){
            if(event.type==sf::Event::KeyPressed){
                if(event.key.code==sf::Keyboard::Escape){
                    Change_Button(-1,0);
                }
                else if(game_setup.key_change[0]==0){
                    // Check availability
                    if(game_setup.Key_Available(player,event.key.code)){
                        player[game_setup.key_change[1]].keyL=event.key.code;
                        Change_Button(-1,0);
                    }
                }
                else if(game_setup.key_change[0]==1){
                    if(game_setup.Key_Available(player,event.key.code)){
                        player[game_setup.key_change[1]].keyR=event.key.code;
                        Change_Button(-1,0);
                    }
                }
                // Update renderer.objects
                renderer.objects.Sync_Players(config,player);
            }
        }
        // End key-changer
        #ifdef DEBUG
        // Auto adder, only in debug
        else if(event.type==sf::Event::KeyPressed&&event.key.code==sf::Keyboard::Space){
            game_setup.Auto_Add_Players(config,game,player);
            renderer.objects.Sync_Players(config,player);
        }
        #endif
        // Name Changer
        if(game.name_change>-1&&event.type==sf::Event::KeyPressed&& (event.key.code==sf::Keyboard::Escape) ){
            renderer.objects.s_names[game.name_change].setActive(false);
            game.name_change=-1;
            // Reset
            renderer.objects.Sync_Players(config,player);
        }
        else if(game.name_change>-1&&event.type==sf::Event::KeyPressed&& (event.key.code==sf::Keyboard::Return) ){
            if(renderer.objects.s_names[game.name_change].getString().getSize()==0){
                renderer.objects.Sync_Players(config,player);
            }
            else{
                player[game.name_change].name=renderer.objects.s_names[game.name_change].getString();
                // Send to others
                if(game.server[1]||game.client[1]){
                    Pending pending;
                    pending.packet << Packet::Name << game.name_change << player[game.name_change].name;
                    pending.send_id.push_back(-1);
                    game.mutex.lock();
                    game.packets.push_back(pending);
                    game.mutex.unlock();
                }
            }
            renderer.objects.s_names[game.name_change].setActive(false);
            game.name_change=-1;
        }
        else if(game.name_change>-1){
            renderer.objects.s_names[game.name_change].Event(event);
        }
    }
    // Play Game
    else if(game.mode==Game::Mode::Play){
        // End game
        if(game.game_finished&&event.type==sf::Event::KeyPressed&&event.key.code==sf::Keyboard::Space){
            game.Quit(config);
            if(game.server[1]){
                // SEND THE SYNCER!
                // All player information
                Pending pending;
                pending.packet << Packet::Sync;
                for(unsigned int i=0;i<player.size();i++){
                    pending.packet << i << player[i].name << player[i].color << player[i].server << player[i].ready;
                }
                pending.send_id.push_back(-1);
                game.mutex.lock();
                game.packets.push_back(pending);
                game.mutex.unlock();
            }
        }
        // New Round
        else if(game.round_finished&&!game.client[1]&&event.type==sf::Event::KeyPressed&&event.key.code==sf::Keyboard::Space){
            game.New_Round(config,player);
        }
        // Pause
        else if(!game.client[1]&&game.countdown_int==0&&event.type==sf::Event::KeyPressed&&event.key.code==sf::Keyboard::Space){
            game.Pause(!game.pause);
        }
    }
}
//
void Main::Main_Menu_Handler(){
    // Create Button
    if(renderer.objects.m_create.Check(renderer.window)){
        renderer.objects.setOptions(game);
        game_setup.Initialize(config,game,player);
        return;
    }
    // Connect
    if(renderer.objects.m_join.Check(renderer.window)){
        // Enable prompt
        renderer.objects.m_ip_prompt.Switch();
    }
    // Connect Confirm button
    if(renderer.objects.m_ip_prompt.Check(renderer.window)){
        //
        if(renderer.objects.m_ip_prompt.getString().getSize()>6){
            game.server_ip=sf::IpAddress(renderer.objects.m_ip_prompt.getString());
            client.Start(config,game,player);
        }
    }
    // Quit
    if(renderer.objects.m_quit.Check(renderer.window)){
        renderer.window.close();
    }
}
//
void Main::Game_Setup_Handler(){
    // Players
    for(unsigned int i=0;i<player.size()&&i<renderer.objects.vector_length;i++){
        if(renderer.objects.s_names[i].Check(renderer.window)){
            // Name Change
            if(player[i].local){
                if(game.name_change==i){
                    if(renderer.objects.s_names[i].getString().getSize()==0){
                        renderer.objects.Sync_Players(config,player);
                    }
                    else{
                        player[i].name=renderer.objects.s_names[i].getString();
                        if(game.server[1]||game.client[1]){
                            Pending pending;
                            pending.packet << Packet::Name << i << player[i].name;
                            pending.send_id.push_back(-1);
                            game.mutex.lock();
                            game.packets.push_back(pending);
                            game.mutex.unlock();
                        }
                    }
                    renderer.objects.s_names[i].setActive(false);
                    game.name_change=-1;
                }
                else if(game.name_change>-1){
                    renderer.objects.s_names[game.name_change].setActive(false);
                    game.name_change=i;
                    renderer.objects.s_names[i].setActive(true);
                }
                else{
                    game.name_change=i;
                    renderer.objects.s_names[i].setActive(true);
                }
            }
        }
        if(player[i].local){
            // Buttons
            if(renderer.objects.s_lbutton[i].Check(renderer.window)){
                Change_Button(0,i);
            }
            if(renderer.objects.s_rbutton[i].Check(renderer.window)){
                Change_Button(1,i);
            }
        }
        // Status
        if(player[i].server&&renderer.objects.s_status[i].getString()!="Server"){
            renderer.objects.s_status[i].setString("Server");
        }
        else if(!player[i].server&&!player[i].local
                && ( (game.server[1] && !server.clients[player[i].id].ready) || (game.client[1] && !player[i].ready ) )
                && renderer.objects.s_status[i].getString()!="Not ready"){
            renderer.objects.s_status[i].setString("Not ready");
        }
        else if(!player[i].server&&!player[i].local
                && ( (game.server[1] && server.clients[player[i].id].ready) || (game.client[1] && player[i].ready ) )
                &&renderer.objects.s_status[i].getString()!="Ready"){
            renderer.objects.s_status[i].setString("Ready");
        }
        // Kick
        // Local: Can kick all. Server: Can kick all but first player. Client: Can kick local players except first
        if( ( (game.client[1] && i!=game.id && player[i].local) || (game.server[1] && i!=0 ) || (!game.server&&!game.client[1] ) ) && renderer.objects.s_kick[i].Check(renderer.window)){
            // Remove from server
            if(game.server[1]){
                if(!player[i].local){
                    // Remove from clients
                    for(unsigned int j=0;j<server.clients[player[i].id].id.size();j++){
                        if(server.clients[player[i].id].id[j]==i){
                            server.clients[player[i].id].id.erase(server.clients[player[i].id].id.begin()+j);
                            break;
                        }
                    }
                    // Update player id's
                    server.Update_Player_id(player,i);
                    // Remove if no player left
                    if(server.clients[player[i].id].id.size()==0){
                        server.selector.remove(*server.clients[player[i].id].socket);
                        server.clients.erase(server.clients.begin()+player[i].id);
                    }
                }
                Pending pending;
                pending.packet << Packet::DCon << i;
                pending.send_id.push_back(-1);
                game.mutex.lock();
                game.packets.push_back(pending);
                game.mutex.unlock();
                //
                game_setup.Remove_Player(game,player,i);
                renderer.objects.Sync_Players(config,player);
            }
            else if(game.client[1]){
                player[i].local=false;
                Pending pending;
                pending.packet << Packet::Remove_Player << i;
                pending.send_id.push_back(-1);
                game.mutex.lock();
                game.packets.push_back(pending);
                game.mutex.unlock();
            }
            break;
        }
    }
    // Options
    if(renderer.objects.s_max10.Check(renderer.window)&&!game.client[1]){
        game.maxpoints=10;
        game.Options_Changed(renderer.objects);
    }
    if(renderer.objects.s_max20.Check(renderer.window)&&!game.client[1]){
        game.maxpoints=20;
        game.Options_Changed(renderer.objects);
    }
    if(renderer.objects.s_max40.Check(renderer.window)&&!game.client[1]){
        game.maxpoints=40;
        game.Options_Changed(renderer.objects);
    }
    if(renderer.objects.s_powerupoff.Check(renderer.window)&&!game.client[1]){
        game.powerup_enabled=false;
        game.Options_Changed(renderer.objects);
    }
    if(renderer.objects.s_powerupon.Check(renderer.window)&&!game.client[1]){
        game.powerup_enabled=true;
        game.Options_Changed(renderer.objects);
    }
    if(!game.client[1]&&!game.server[1]){
        if(renderer.objects.s_countdownoff.Check(renderer.window)){
            game.countdown_enabled=false;
            game.Options_Changed(renderer.objects);
        }
        if(renderer.objects.s_countdownon.Check(renderer.window)){
            game.countdown_enabled=true;
            game.Options_Changed(renderer.objects);
        }
    }
    else{
        if(renderer.objects.s_multiple_playersoff.Check(renderer.window)&&!game.client[1]){
            game.multiple_players_enabled=false;
            game.Options_Changed(renderer.objects);
        }
        if(renderer.objects.s_multiple_playerson.Check(renderer.window)&&!game.client[1]){
            game.multiple_players_enabled=true;
            game.Options_Changed(renderer.objects);
        }
    }
    // Buttons
    if(player.size()<config.max_players&& ( (game.client[1]&&!client.ready) || !game.client[1] ) && renderer.objects.s_add.Check(renderer.window)){
        // Send request
        if(game.client[1]){
            Pending pending;
            pending.packet << Packet::Request_Player;
            game.mutex.lock();
            game.packets.push_back(pending);
            game.mutex.unlock();
        }
        else{
            // Add new player
            game_setup.Add_Player(game,player);
            renderer.objects.Sync_Players(config,player);
            // Send to clients
            if(game.server[1]){
                Pending pending;
                pending.packet << Packet::Sync;
                for(unsigned int i=0;i<player.size();i++){
                    pending.packet << i << player[i].name << player[i].color << player[i].server << player[i].ready;
                }
                pending.send_id.push_back(-1);
                game.mutex.lock();
                game.packets.push_back(pending);
                game.mutex.unlock();
            }
        }
    }
    // Server Text
    if(game.server[1]&&renderer.objects.s_server.getString()=="Start Server"){
        renderer.objects.s_server.setString("Local: " + sf::IpAddress::getLocalAddress().toString());
    }
    else if(game.client[1]&&renderer.objects.s_server.getString()!= "Server: " + game.server_ip.toString() ){
        renderer.objects.s_server.setString("Server: " + game.server_ip.toString());
    }
    else if(!game.server[1]&&!game.client[1]&&renderer.objects.s_server.getString()!="Start Server"){
        renderer.objects.s_server.setString("Start Server");
    }
    // Server Button
    if(renderer.objects.s_server.Check(renderer.window)){
        // Depending on situation: Start Server, Stop Server or do nothing.
        if(game.client[1]){
            //
        }
        else if(!game.server[1]){
            // Start Server
            server.Start(config,game_setup,game,player);
        }
        else{
            server.Shutdown(game,player,game_setup);
        }
    }
    // Start Button
    if(renderer.objects.s_start.Check(renderer.window)){
        if(!game.client[1]){
            // Otherwise check if all players are ready and start
            game_setup.Start_Game(config,game,player,server);
        }
        else{
            // Client Things
            client.Ready(game,player);
        }
    }
    if( (game.client[1] && !client.ready) && renderer.objects.s_start.getString()!="Ready" ){
        renderer.objects.s_start.setString("Ready!");
        renderer.objects.s_start.setPosition(config.window_width/2-renderer.objects.s_start.getLocalBounds().width/2,config.window_height-100);
    }
    else if(game.client[1]&&client.ready&&renderer.objects.s_start.getString()!="Waiting"){
        renderer.objects.s_start.setString("Waiting!");
        renderer.objects.s_start.setPosition(config.window_width/2-renderer.objects.s_start.getLocalBounds().width/2,config.window_height-100);
    }
    else if(renderer.objects.s_start.getString()!="Start Game"){
        renderer.objects.s_start.setString("Start Game");
        renderer.objects.s_start.setPosition(config.window_width/2-renderer.objects.s_start.getLocalBounds().width/2,config.window_height-100);
    }
//
    if(renderer.objects.s_quit.Check(renderer.window)){
        // To main menu
        game_setup.Quit(config,game,player);
    }
}
//
void Main::Change_Button(const int &button,unsigned const int &pl){
    // Set game variables
    game_setup.key_change[0]=button;
    game_setup.key_change[1]=pl;
    // Set graphics
    for(unsigned int i=0;i<player.size()&&i<renderer.objects.vector_length;i++){
        if(button!=-1&&pl==i){
            if(button==0){
                renderer.objects.s_lbutton[i].setActive(true);
                renderer.objects.s_rbutton[i].setActive(false);
            }
            else{
                renderer.objects.s_lbutton[i].setActive(false);
                renderer.objects.s_rbutton[i].setActive(true);
            }
        }
        else{
            renderer.objects.s_lbutton[i].setActive(false);
            renderer.objects.s_rbutton[i].setActive(false);
        }
    }
}
//
void Main::Play_Handler(){
    if(renderer.objects.g_quit.Check(renderer.window)){
        game.Quit(config);
    }
    // Change render objects
    for(unsigned int i=0;i<player.size()&&i<renderer.objects.vector_length;i++){
        renderer.objects.g_score[i].setString(int2string(player[i].points));
    }
    // Update round number
    if(renderer.objects.g_round[1].getString()!=int2string(game.round)){
        renderer.objects.g_round[1].setString(int2string(game.round));
    }
    // Countdown timer
    if(game.countdown_int>0){
        renderer.objects.g_countdown.setString(int2string(game.countdown_int));
        renderer.objects.g_countdown.setPosition(config.window_width/2-config.statuswidth/2-renderer.objects.g_countdown.getLocalBounds().width/2,config.window_height/2-renderer.objects.g_countdown.getLocalBounds().height/2);
        if(game.countdown_int==3){
            renderer.objects.g_countdown.setColor(sf::Color::Red);
        }
        else if(game.countdown_int==2){
            renderer.objects.g_countdown.setColor(sf::Color::Yellow);
        }
        else{
            renderer.objects.g_countdown.setColor(sf::Color::Green);
        }
    }
    //
    if(game.round_finished&&!game.end_message_set){
        if(game.game_finished){
            renderer.objects.g_end_round_message.setString(player[game.round_winner].name + " has won the game!");
            renderer.objects.g_end_round_message.setColor(player[game.round_winner].color);
        }
        else if(game.round_winner==-1){
            renderer.objects.g_end_round_message.setString("Round finished!");
            renderer.objects.g_end_round_message.setColor(sf::Color::Red);
        }
        else{
            renderer.objects.g_end_round_message.setString(player[game.round_winner].name + " has won the round!");
            renderer.objects.g_end_round_message.setColor(player[game.round_winner].color);
        }
        renderer.objects.g_end_round_message.setPosition(config.window_width/2-config.statuswidth/2-renderer.objects.g_end_round_message.getLocalBounds().width/2,config.window_height/2-renderer.objects.g_end_round_message.getLocalBounds().height/2);
        game.end_message_set=true;
    }
}
