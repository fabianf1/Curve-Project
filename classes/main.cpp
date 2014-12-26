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
        //if(game.mode_lock.try_lock()){
            if(game.mode==Game::Mode::Main_Menu){
                Main_Menu_Handler();
            }
            else if(game.mode==Game::Mode::Setup){
                Game_Setup_Handler();
            }
            else if(game.mode==Game::Mode::Play){
                Play_Handler();
            }
            //game.mode_lock.unlock();
        //}
        // Check if things need to be started
        /*if(game.update_thread[0]&&!game.update_thread[1]){
            //Start_Game_Update(config,game,player);
        }
        else if(game.server[0]&&!game.server[1]){
            //server.Start(config,game,player);
        }
        else if(game.client[0]&&!game.client[1]){
            //client.Start(config,game,player);
        }*/
        // Check if new player is added
        if(renderer.objects.vector_length<player.size()||renderer.objects.vector_length>player.size()){
            renderer.objects.Sync_Players(config,game,player);
        }
        // Delay function
        Sleep(config.main_thread_min_time,game.main_thread_clock);
        //
        /* When switching it goes wrong somewhere I think. No I am sure. Stop the handlers for a brief moment when switching to solve!*/
        /*if(game.packetclock.getElapsedTime().asSeconds()>4.0&&game.packetclock.getElapsedTime().asSeconds()<4.5){
            game.mode=Game::Mode::Setup;
        }*/
    }
    // Shutdown procedure
    game.update_thread[0]=false;
    game.server[0]=false;
    game.client[0]=false;
    game.update_thread[2]=true;
    game.server[2]=true;
    game.client[2]=true;
    //game.Shutdown();
    server.Shutdown(game);
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
        game.Pause(config,true);
    }
    // Regain Focus
    /*else if(event.type == sf::Event::GainedFocus){
        Pause_Game(config,game,window,false);
    }*/
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
                    game_setup.key_change[0]=-1;
                }
                else if(game_setup.key_change[0]==0){
                    // Check availability
                    if(game_setup.Key_Available(player,event.key.code)){
                        player[game_setup.key_change[1]].keyL=event.key.code;
                        game.keychange[0]=-1;
                    }
                }
                else if(game_setup.key_change[0]==1){
                    if(game_setup.Key_Available(player,event.key.code)){
                        player[game_setup.key_change[1]].keyR=event.key.code;
                        game_setup.key_change[0]=-1;
                    }
                }
                // Update renderer.objects
                renderer.objects.Sync_Players(config,game,player);
            }
        }
        // End key-changer
        #ifdef DEBUG
        // Auto adder, only in debug
        else if(event.type==sf::Event::KeyPressed&&event.key.code==sf::Keyboard::Space){
            game_setup.Auto_Add_Players(config,game,player);
            renderer.objects.Sync_Players(config,game,player);
        }
        #endif
    }
    // Play Game
    else if(game.mode==Game::Mode::Play){
        // End game
        if(game.game_finished&&event.type==sf::Event::KeyPressed&&event.key.code==sf::Keyboard::Space){
            game.Quit(config,player);
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
            game.New_Round(config,game,player);
            // Update round number
            //renderer.objects.g_round[1].setString(int2string(game.round));
        }
        // Pause
        else if(!game.server[1]&&!game.client[1]&&event.type==sf::Event::KeyPressed&&event.key.code==sf::Keyboard::Space){
            game.pause=!game.pause;
            game.game_clock.restart();
            /*if(game.pause){Pause_Game(config,game,false);}
            else{Pause_Game(config,game,true);}*/
        }
    }
/*
    // Setup screen MP
    else if(game.mode==Game::Mode::Setup_MP){
        if(event.type==sf::Event::KeyPressed&&(event.key.code==sf::Keyboard::Return||event.key.code==sf::Keyboard::Escape)){
            if(game.name_change){
                player[game.id].Name_Packet(game);
                game.name_change=false;
            }
            game.keychange[1]=-1;
        }
        // Name Change
        else if(game.name_change){
            // Text Entered
            if(event.type==sf::Event::TextEntered){
                if(event.text.unicode==8&&player[game.id].name.getSize()>0){
                    player[game.id].name.erase(player[game.id].name.getSize()-1,1);
                }
                else if(event.text.unicode>31&&event.text.unicode < 127&&player[game.id].name.getSize()<16&&player[game.id].name.getSize()<=10){
                    player[game.id].name+=static_cast<char>(event.text.unicode);
                }
            } // End Textentered
        }
        // End Name Change
    }
    */
}
//
void Main::Main_Menu_Handler(){
    // Create Button
    if(renderer.objects.m_create.Check(renderer.window)){
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
        sf::sleep(sf::milliseconds(500));
        renderer.window.close();
    }
}
//
void Main::Game_Setup_Handler(){
    // Players
    for(unsigned int i=0;i<player.size()&&i<renderer.objects.vector_length;i++){
        if(renderer.objects.s_names[i].Check(renderer.window)){
            // Name Change
        }
        if(renderer.objects.s_lbutton[i].Check(renderer.window)){
            game_setup.key_change[0]=0;
            game_setup.key_change[1]=i;
        }
        if(renderer.objects.s_rbutton[i].Check(renderer.window)){
            game_setup.key_change[0]=1;
            game_setup.key_change[1]=i;
        }
        if(renderer.objects.s_kick[i].Check(renderer.window)){
            game_setup.Remove_Player(config,game,player,i);
            renderer.objects.Sync_Players(config,game,player);
        }
        // Status
        if(player[i].server&&renderer.objects.s_status[i].getString()!="Server"){
            renderer.objects.s_status[i].setString("Server");
        }
        else if(!player[i].server&&!player[i].local&&!player[i].ready&&renderer.objects.s_status[i].getString()!="Not ready"){
            renderer.objects.s_status[i].setString("Not ready");
        }
        else if(!player[i].server&&!player[i].local&&player[i].ready&&renderer.objects.s_status[i].getString()!="Ready"){
            renderer.objects.s_status[i].setString("Ready");
        }
    }
    // Options
    if(renderer.objects.s_max10.Check(renderer.window)){

    }
    if(renderer.objects.s_max20.Check(renderer.window)){

    }
    if(renderer.objects.s_max40.Check(renderer.window)){

    }
    if(renderer.objects.s_powerupoff.Check(renderer.window)){

    }
    if(renderer.objects.s_powerupon.Check(renderer.window)){

    }
    // Buttons
    if(renderer.objects.s_add.Check(renderer.window)){
        // Add new player
        if(!game.client[1]){
            game_setup.Add_Player(config,game,player);
            renderer.objects.Sync_Players(config,game,player);
        }
        else{
            //
        }
    }
    // Server Text
    if(game.server[1]&&renderer.objects.s_server.getString()=="Start Server"){
        renderer.objects.s_server.setString("Local: " + sf::IpAddress::getLocalAddress().toString());
    }
    else if(!game.server[1]&&renderer.objects.s_server.getString()!="Start Server"){
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
            server.Shutdown(game);
        }
    }
    // Start Button
    if(renderer.objects.s_start.Check(renderer.window)){
        if(!game.client[1]){
            // Otherwise check if all players are ready and start
            game_setup.Start_Game(config,game,player);
        }
        else{
            // Client Things
            client.Ready(game,player);
        }
    }
    if( (game.client[1] && !client.ready) && renderer.objects.s_start.getString()!="Ready" ){
        renderer.objects.s_start.setString("Ready!");
    }
    else if(game.client[1]&&client.ready&&renderer.objects.s_start.getString()!="Waiting"){
        renderer.objects.s_start.setString("Waiting!");
    }
    else if(renderer.objects.s_start.getString()!="Start Game"){
        renderer.objects.s_start.setString("Start Game");
    }
//
    if(renderer.objects.s_quit.Check(renderer.window)){
        // To main menu
        game_setup.Quit(config,game,player);
    }
}
//
void Main::Play_Handler(){
    if(renderer.objects.g_quit.Check(renderer.window)){
        game.Quit(config,player);
    }
    // Change render objects
    for(unsigned int i=0;i<player.size()&&i<renderer.objects.vector_length;i++){
        renderer.objects.g_score[i].setString(int2string(player[i].points));
    }
    if(game.frame % 15 == 0){
        // Update round number
        renderer.objects.g_round[1].setString(int2string(game.round));
        renderer.objects.g_frame[1].setString(int2string(game.frame));
        // Set FPS
        /*sf::Time elapsed = game.fps_clock.restart();
        game.last_fps=int ( 30/ (elapsed.asSeconds()) );
        renderer.objects.g_fps[1].setString(int2string(game.last_fps));*/
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
    // Maybe add set marker so it isn't set every time; No Do it
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
