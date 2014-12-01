// Contains functions and constructors for the Renderer class
// Needed Header
#include "renderer.h"
//
void Renderer::Start(const Config &config,Game &game,Player player[]){
    // Window
    window.create(sf::VideoMode(config.window_width, config.window_height), config.title, config.window_style, sf::ContextSettings(24,8,config.window_antialising));
    window.setIcon(202,202,config.icon.getPixelsPtr());
    window.clear(config.window_backgroundcolor);
    window.display();
    // Limit fps
    window.setFramerateLimit(config.fps);
    // Start Render thread
    window.setActive(false);
    thread = std::thread(&Renderer::Thread,this,std::cref(config),std::ref(game),player);
    // Done
}
// Render Thread Function
void Renderer::Thread(const Config &config,Game &game,Player player[]){
    std::cout << "Render Thread Started" << std::endl;
    // Start main loop
    while(window.isOpen()){
        window.clear(config.window_backgroundcolor);
        //
        if(game.mode==Game::Mode::Main_Menu){
            Main_Men(config,game,player,main_menu);
        }
        else if(game.mode==Game::Mode::Server_IP){
            Main_Men(config,game,player,main_menu);
            Server_IP(config,game,player,main_menu);
        }
        else if(game.mode==Game::Mode::Setup_SP){
            Setup_SP(config,game,player,game_setup);
        }
        else if(game.mode==Game::Mode::Setup_MP){
            Setup_MP(config,game,player,game_setup_mp);
        }
        else if(game.mode==Game::Mode::Play){
            Gam(config,game,player,game_store);
            Game_Status_Menu(config,game,player,game_store);
            if(game.powerup_enabled){PowerUp(config,game,sprite);}
        }
        else if(game.mode==Game::Mode::Play_MP){
            Game_MP(config,game,player,game_store);
            Game_Status_Menu(config,game,player,game_store);
            PowerUp(config,game,sprite);
        }
        //
        window.display();
        game.frame++;
    }
    std::cout << "Render Thread Stopped" << std::endl;
}
//
void Renderer::Main_Men(const Config &config,Game &game,Player player[],Main_Menu &main_menu){
    // Title
    window.draw(main_menu.title);
    // Buttons
    // Start Game
    if(main_menu.start_game.Check(game,window)){
        Initialize_Game_Setup(config,game,player);
    }
    window.draw(main_menu.start_game);
    // Server
    if(main_menu.server.Check(game,window)){
        game.server[0]=true;
    }
    window.draw(main_menu.server);
    // Client
    if(main_menu.client.Check(game,window)){
        //game.client[0]=true;
        //game.server_ip.clear();
        game.id=-1;
        game.mode=Game::Mode::Server_IP;
    }
    window.draw(main_menu.client);
    // Quit
    if(main_menu.quit.Check(game,window)){
        sf::sleep(sf::milliseconds(500));
        window.close();
    }
    window.draw(main_menu.quit);
}
//
void Renderer::Server_IP(const Config &config,Game &game,Player player[],Main_Menu &main_menu){
    window.draw(main_menu.outline);
    window.draw(main_menu.connect_title);
    //
    main_menu.ip.setString(game.server_ip);
    main_menu.ip.setPosition( (config.window_width/2)-(main_menu.ip.getLocalBounds().width/2) ,config.window_height/2-main_menu.outline.getLocalBounds().height/2 + 40);
    window.draw(main_menu.ip);
    //
    if(main_menu.connect.Check(game,window)){
        game.client[0]=true;
    }
    window.draw(main_menu.connect);
}
//
void Renderer::Setup_SP(const Config &config,Game &game,Player player[],Game_Setup &game_setup){
    // Title
    window.draw(game_setup.title);
    /* Player button setup part */
    // Name
    window.draw(game_setup.name);
    // Left
    window.draw(game_setup.left);
    // Right
    window.draw(game_setup.right);
    // Names and buttons
    for(int i=0;i<MAX_PLAYERS;i++){
        /*
        When player is disabled: Name, Left and right lower alpha(125)
        When disabled player is selected: Name normal alpha and style italic
        When Player is enabled:  Name, left and right normal alpha
        When enabled player is selected: Alpha normal and italic
        When changing key: Key that is changed Bold, possibly different color?
        */
        // Setup color and styles
        sf::Color color,s_color;
        color=player[i].color;
        s_color=player[i].color;
        if(!player[i].enabled){ color.a=125;}
        //
        int style,s_style,k_style;
        style=sf::Text::Regular;
        s_style=sf::Text::Italic;
        k_style=sf::Text::Bold; // Key change style
        // Player Name
        game_setup.names[i].setColors(color,s_color);
        game_setup.names[i].setStyles(style,s_style);
        //
        if(game_setup.names[i].Check(game,window)){
            Switch_Player(game,player,i);
        }
        //
        window.draw(game_setup.names[i]);
        // Player Left
        game_setup.lbutton[i].setString(getKeyName(player[i].keyL));
        if(game.keychange[0]==i&&game.keychange[1]==1){
            game_setup.lbutton[i].setStyles(k_style,k_style);
        }
        else{
            game_setup.lbutton[i].setStyles(style,s_style);
        }
        game_setup.lbutton[i].setColors(color,s_color);
        //
        if(game_setup.lbutton[i].Check(game,window)&&player[i].enabled){
            if(game.keychange[0]==i&&game.keychange[1]==1){
                game.keychange[0]=-1;
            }
            else{
                game.keychange[0]=i;
                game.keychange[1]=1;
            }
        }
        //
        window.draw(game_setup.lbutton[i]);
        // Player Right
        game_setup.rbutton[i].setString(getKeyName(player[i].keyR));
        //
        if(game.keychange[0]==i&&game.keychange[1]==2){
            game_setup.rbutton[i].setStyles(k_style,k_style);
        }
        else{
           game_setup.rbutton[i].setStyles(style,s_style);
        }
        game_setup.rbutton[i].setColors(color,s_color);
        //
        if(game_setup.rbutton[i].Check(game,window)&&player[i].enabled){
            if(game.keychange[0]==i&&game.keychange[1]==2){
                game.keychange[0]=-1;
            }
            else{
                game.keychange[0]=i;
                game.keychange[1]=2;
            }
        }
        //
         window.draw(game_setup.rbutton[i]);
    }
    // End Player button select
    // Some config options
    window.draw(game_setup.options);
    // Max Points
    window.draw(game_setup.maxpoints);
    // Prepare color
    sf::Color color=sf::Color::Red;
    color.a=125;
    // Point 10
    if(game.maxpoints==10){game_setup.max10.setColors(sf::Color::Red,sf::Color::Red);}
    else{game_setup.max10.setColors(color,sf::Color::Red);}
    //
    if(game_setup.max10.Check(game,window)){
        game.maxpoints=10;
    }
    window.draw(game_setup.max10);
    // Point 20
    if(game.maxpoints==20){game_setup.max20.setColors(sf::Color::Red,sf::Color::Red);}
    else{game_setup.max20.setColors(color,sf::Color::Red);}
    //
    if(game_setup.max20.Check(game,window)){
        game.maxpoints=20;
    }
    window.draw(game_setup.max20);
    // Point 40
    if(game.maxpoints==40){game_setup.max40.setColors(sf::Color::Red,sf::Color::Red);}
    else{game_setup.max40.setColors(color,sf::Color::Red);}
    if(game_setup.max40.Check(game,window)){
        game.maxpoints=40;
    }
    window.draw(game_setup.max40);
    // End Max Points
    // Powerups activated?
    window.draw(game_setup.powerups);
    // Enabled
    if(game.powerup_enabled){game_setup.powerupon.setColors(sf::Color::Red,sf::Color::Red);}
    else{game_setup.powerupon.setColors(color,sf::Color::Red);}
    if(game_setup.powerupon.Check(game,window)){
        game.powerup_enabled=true;
    }
    window.draw(game_setup.powerupon);
    // Disabled
    if(!game.powerup_enabled){game_setup.powerupoff.setColors(sf::Color::Red,sf::Color::Red);}
    else{game_setup.powerupoff.setColors(color,sf::Color::Red);}
    if(game_setup.powerupoff.Check(game,window)){
        game.powerup_enabled=false;
    }
    window.draw(game_setup.powerupoff);
    // End powerups
    // End config
    // Start game button
    if(game_setup.start.Check(game,window)){
        Initialize_Game(config,game,player);
    }
    window.draw(game_setup.start);
    if(game_setup.quit.Check(game,window)){
        game.mode=Game::Mode::Main_Menu;
    }
    window.draw(game_setup.quit);
}
//
void Renderer::Setup_MP(const Config &config,Game &game,Player player[],Game_Setup_MP &game_setup){
     // Title
    window.draw(game_setup.title);
    // Name
    window.draw(game_setup.name);
    // Part to change name
    if(game_setup.name_field.Check(game,window)){
        if(game.name_change){
                game.name_change=false;
                player[game.id].Name_Packet(game);
        }
        else{game.name_change=true;}
    }
    if(game.id>-1){game_setup.name_field.setString(player[game.id].name);}
    //
    if(game.name_change){
        game_setup.name_field.setStyles(sf::Text::Bold,sf::Text::Bold);
    }
    else{
        game_setup.name_field.setStyles(sf::Text::Regular,sf::Text::Italic);
    }
    //
    window.draw(game_setup.name_field);
    if(game.name_change&&game.frame%config.fps<config.fps/2){
        int x=game_setup.name_field.getGlobalBounds().left+game_setup.name_field.getGlobalBounds().width;
        game_setup.name_pointer.setPosition(x, game_setup.name_pointer.getPosition().y);
        window.draw(game_setup.name_pointer);
    }
    // End name
    // Left
    window.draw(game_setup.left);
    if(game.id>-1){game_setup.keyL.setString(getKeyName(player[game.id].keyL));}
    if(game_setup.keyL.Check(game,window)){
        if(game.keychange[1]==1){
            game.keychange[1]=-1;
        }
        else{
            if(game.client[1]&&player[game.id].ready){
                Client_Ready(game,player);
            }
            game.keychange[1]=1;
        }
    }
    //
    if(game.keychange[1]==1){
        game_setup.keyL.setStyles(sf::Text::Bold,sf::Text::Bold);
    }
    else{
        game_setup.keyL.setStyles(sf::Text::Regular,sf::Text::Italic);
    }
    //
    window.draw(game_setup.keyL);
    // Right
    window.draw(game_setup.right);
    if(game.id>-1){game_setup.keyR.setString(getKeyName(player[game.id].keyR));}
    if(game_setup.keyR.Check(game,window)){
        if(game.keychange[1]==2){
            game.keychange[1]=-1;
        }
        else{
            if(game.client[1]&&player[game.id].ready){
                Client_Ready(game,player);
            }
            game.keychange[1]=2;
        }
    }
    //
    if(game.keychange[1]==2){
        game_setup.keyR.setStyles(sf::Text::Bold,sf::Text::Bold);
    }
    else{
        game_setup.keyR.setStyles(sf::Text::Regular,sf::Text::Italic);
    }
    //
    window.draw(game_setup.keyR);
    // Color
    game_setup.color.setColor(config.linecolors[game.id]);
    window.draw(game_setup.color);
    // Other players
    window.draw(game_setup.other_names);
    for(int i=0,j=0;i<MAX_PLAYERS;i++){
        if(player[i].enabled&&i!=game.id){
            game_setup.names[j].setString(player[i].name);
            game_setup.names[j].setColor(config.linecolors[i]);
            window.draw(game_setup.names[j]);
            if(i==0){
                game_setup.ready[j].setString("Server");
            }
            else if(player[i].ready){
                game_setup.ready[j].setString("Ready");
            }
            else{
                game_setup.ready[j].setString("Not ready");
            }
            window.draw(game_setup.ready[j]);
            j++;
        }
    }
    //
    // Some config options
    window.draw(game_setup.options);
    // Max Points
    window.draw(game_setup.maxpoints);
    // Prepare color
    sf::Color color=sf::Color::Red;
    color.a=125;
    // Point 10
    if(game.maxpoints==10){game_setup.max10.setColors(sf::Color::Red,sf::Color::Red);}
    else{game_setup.max10.setColors(color,sf::Color::Red);}
    //
    if(game_setup.max10.Check(game,window)&&game.server[1]){
        game.maxpoints=10;
        Config_Package(game);
    }
    window.draw(game_setup.max10);
    // Point 20
    if(game.maxpoints==20){game_setup.max20.setColors(sf::Color::Red,sf::Color::Red);}
    else{game_setup.max20.setColors(color,sf::Color::Red);}
    //
    if(game_setup.max20.Check(game,window)&&game.server[1]){
        game.maxpoints=20;
        Config_Package(game);
    }
    window.draw(game_setup.max20);
    // Point 40
    if(game.maxpoints==40){game_setup.max40.setColors(sf::Color::Red,sf::Color::Red);}
    else{game_setup.max40.setColors(color,sf::Color::Red);}
    if(game_setup.max40.Check(game,window)&&game.server[1]){
        game.maxpoints=40;
        Config_Package(game);
    }
    window.draw(game_setup.max40);
    // End Max Points
    // Powerups activated?
    window.draw(game_setup.powerups);
    // Enabled
    if(game.powerup_enabled){game_setup.powerupon.setColors(sf::Color::Red,sf::Color::Red);}
    else{game_setup.powerupon.setColors(color,sf::Color::Red);}
    if(game_setup.powerupon.Check(game,window)&&game.server[1]){
        game.powerup_enabled=true;
        Config_Package(game);
    }
    window.draw(game_setup.powerupon);
    // Disabled
    if(!game.powerup_enabled){game_setup.powerupoff.setColors(sf::Color::Red,sf::Color::Red);}
    else{game_setup.powerupoff.setColors(color,sf::Color::Red);}
    if(game_setup.powerupoff.Check(game,window)&&game.server[1]){
        game.powerup_enabled=false;
        Config_Package(game);
    }
    window.draw(game_setup.powerupoff);
    // End powerups
    // End config
    // Start Button
    if(game.server[1]){
        game_setup.start.setString("Start Game");
    }
    else if(game.id>-1){
        if(player[game.id].ready){
            game_setup.start.setString("Waiting...");
        }
        else{
            game_setup.start.setString("Set Ready");
        }
    }
    if(game_setup.start.Check(game,window)){
        if(game.server[1]){
            Server_Start_Game(config,game,player);
        }
        else{
            Client_Ready(game,player);
        }
    }
    window.draw(game_setup.start);
    // Quit Button
    if(game_setup.quit.Check(game,window)){
        game.mode=Game::Mode::Main_Menu;
        if(game.server[1]){
            game.server[2]=true;
        }
        if(game.client[1]){
            game.client[2]=true;
        }
    }
    window.draw(game_setup.quit);
    //
    if(game.server[1]){
        game_setup.status.setString("Server. Local IP: " + game.server_ip);
    }
    else if(game.client[1]){
        game_setup.status.setString("Client. Server IP: " + game.server_ip);
    }
    else{
        game_setup.status.setString("Not connected?");
    }
    window.draw(game_setup.status);
}
//
void Renderer::Gam(const Config &config,Game &game,Player player[],Game_Store &game_store){
    for(int i=0;i<MAX_PLAYERS;i++){
        if(!player[i].enabled){continue;}// Don't process disabled players
        window.draw(player[i].line);
        // Draw the circle
        window.draw(player[i].circle);
    }
    //
    sf::Color color=config.wallcolor;
    // Check if walls away effect is there and change wallcolor(Decouple from fps?)
    if(game.wallsaway){
        int remainder=(game.frame%config.fps)-config.fps/2;
        if(remainder<0){
            color.a=255/(config.fps/2)*(-1)*remainder;
        }
        else{
            color.a=255/(config.fps/2)*remainder;
        }
    }
    // Draw walls
    game_store.leftwall.setFillColor(color);
    window.draw(game_store.leftwall);
    game_store.topwall.setFillColor(color);
    window.draw(game_store.topwall);
    game_store.rightwall.setFillColor(color);
    window.draw(game_store.rightwall);
    game_store.bottomwall.setFillColor(color);
    window.draw(game_store.bottomwall);
    // Round finished message
    if(game.round_finished){
        // Create message
        if(game.game_finished){
            game_store.end_round_message.setString(player[game.round_winner].name + " has won the game!");
            game_store.end_round_message.setColor(player[game.round_winner].color);
        }
        else{
            if(game.round_winner>-1){
                game_store.end_round_message.setString(player[game.round_winner].name + " has won the round!");
                game_store.end_round_message.setColor(player[game.round_winner].color);
            }
            else{
                game_store.end_round_message.setString("Round finshed");
                game_store.end_round_message.setColor(sf::Color::Red);
            }
        }
        // Text in middle of line screen
        game_store.end_round_message.setPosition(( config.window_width-config.statuswidth)/2 - game_store.end_round_message.getLocalBounds().width/2 , config.window_height/2 - game_store.end_round_message.getLocalBounds().height/2);
        //draw
        window.draw(game_store.end_round_message);
    }
}
//
void Renderer::Game_MP(const Config &config,Game &game,Player player[],Game_Store &game_store){
    // Draw players
    for(int i=0;i<MAX_PLAYERS;i++){
        if(!player[i].enabled){continue;}// Don't process disabled players
        window.draw(player[i].line);
        // Draw the circle
        window.draw(player[i].circle);
    }
    //
    sf::Color color=config.wallcolor;
    // Check if walls away effect is there and change wallcolor(Decouple from fps?)
    if(game.wallsaway){
        int remainder=(game.frame%config.fps)-config.fps/2;
        if(remainder<0){
            color.a=255/(config.fps/2)*(-1)*remainder;
        }
        else{
            color.a=255/(config.fps/2)*remainder;
        }
    }
    // Draw walls
    game_store.leftwall.setFillColor(color);
    window.draw(game_store.leftwall);
    game_store.topwall.setFillColor(color);
    window.draw(game_store.topwall);
    game_store.rightwall.setFillColor(color);
    window.draw(game_store.rightwall);
    game_store.bottomwall.setFillColor(color);
    window.draw(game_store.bottomwall);
    // Draw countdown
    if(game.countdown_int>0){
        game_store.countdown.setString(int2string(game.countdown_int));
        if(game.countdown_int==3){
            game_store.countdown.setColor(sf::Color::Red);
        }
        else if(game.countdown_int==2){
            game_store.countdown.setColor(sf::Color(255, 127, 0));
        }
        else{
            game_store.countdown.setColor(sf::Color::Green);
        }
        game_store.countdown.setPosition(( config.window_width-config.statuswidth)/2 - game_store.countdown.getLocalBounds().width/2 , config.window_height/2 - game_store.countdown.getLocalBounds().height/2);
        window.draw(game_store.countdown);
    }
    // Round finished message
    if(game.round_finished){
        // Create message
        if(game.game_finished){
            game_store.end_round_message.setString(player[game.round_winner].name + " has won the game!");
            game_store.end_round_message.setColor(player[game.round_winner].color);
        }
        else{
            if(game.round_winner>-1){
                game_store.end_round_message.setString(player[game.round_winner].name + " has won the round!");
                game_store.end_round_message.setColor(player[game.round_winner].color);
            }
            else{
                game_store.end_round_message.setString("Round finshed");
                game_store.end_round_message.setColor(sf::Color::Red);
            }
        }
        // Text in middle of line screen
        game_store.end_round_message.setPosition(( config.window_width-config.statuswidth)/2 - game_store.end_round_message.getLocalBounds().width/2 , config.window_height/2 - game_store.end_round_message.getLocalBounds().height/2);
        //draw
        window.draw(game_store.end_round_message);
    }
}
//
void Renderer::PowerUp(const Config &config,Game &game,Powerup_Sprite &sprite){
    for(unsigned int i=0;i<game.powerup.size();i++){
        // find opacity
        int opacity=255;
        if(game.powerup[i].time1<config.powerup_fade_time){
            opacity=(game.powerup[i].time1/config.powerup_fade_time)*255;
        }
        else if(game.powerup[i].time2<config.powerup_fade_time){
            opacity=(game.powerup[i].time2/config.powerup_fade_time)*255;
        }
        sf::Color color(255,255,255,opacity);
        // Choose Sprite
        // Fast
        if(game.powerup[i].type==0){
            if(game.powerup[i].impact==0){
                sprite.fastgreen.setPosition( game.powerup[i].x-config.powerup_radius , game.powerup[i].y-config.powerup_radius );
                sprite.fastgreen.setColor(color);
                window.draw(sprite.fastgreen);
            }
            else if(game.powerup[i].impact==1){
                sprite.fastred.setPosition( game.powerup[i].x-config.powerup_radius , game.powerup[i].y-config.powerup_radius );
                sprite.fastred.setColor(color);
                window.draw(sprite.fastred);
            }
        }
        // Slow
        else if(game.powerup[i].type==1){
            if(game.powerup[i].impact==0){
                sprite.slowgreen.setPosition( game.powerup[i].x-config.powerup_radius , game.powerup[i].y-config.powerup_radius );
                sprite.slowgreen.setColor(color);
                window.draw(sprite.slowgreen);
            }
            else if(game.powerup[i].impact==1){
                sprite.slowred.setPosition( game.powerup[i].x-config.powerup_radius , game.powerup[i].y-config.powerup_radius );
                sprite.slowred.setColor(color);
                window.draw(sprite.slowred);
            }
        }
        // Small
        else if(game.powerup[i].type==2){
            if(game.powerup[i].impact==0){
                sprite.smallgreen.setPosition( game.powerup[i].x-config.powerup_radius , game.powerup[i].y-config.powerup_radius );
                sprite.smallgreen.setColor(color);
                window.draw(sprite.smallgreen);
            }
            else if(game.powerup[i].impact==1){
                sprite.smallred.setPosition( game.powerup[i].x-config.powerup_radius , game.powerup[i].y-config.powerup_radius );
                sprite.smallred.setColor(color);
                window.draw(sprite.smallred);
            }
        }
        // Big
        else if(game.powerup[i].type==3){
            if(game.powerup[i].impact==0){
                sprite.biggreen.setPosition( game.powerup[i].x-config.powerup_radius , game.powerup[i].y-config.powerup_radius );
                sprite.biggreen.setColor(color);
                window.draw(sprite.biggreen);
            }
            else if(game.powerup[i].impact==1){
                sprite.bigred.setPosition( game.powerup[i].x-config.powerup_radius , game.powerup[i].y-config.powerup_radius );
                sprite.bigred.setColor(color);
                window.draw(sprite.bigred);
            }
        }
        // Right Angle
        else if(game.powerup[i].type==4){
            if(game.powerup[i].impact==0){
                sprite.anglegreen.setPosition( game.powerup[i].x-config.powerup_radius , game.powerup[i].y-config.powerup_radius );
                sprite.anglegreen.setColor(color);
                window.draw(sprite.anglegreen);
            }
            else if(game.powerup[i].impact==1){
                sprite.anglered.setPosition( game.powerup[i].x-config.powerup_radius , game.powerup[i].y-config.powerup_radius );
                sprite.anglered.setColor(color);
                window.draw(sprite.anglered);
            }
        }
        // Clear
        else if(game.powerup[i].type==5){
            sprite.clearblue.setPosition( game.powerup[i].x-config.powerup_radius , game.powerup[i].y-config.powerup_radius );
            sprite.clearblue.setColor(color);
            window.draw(sprite.clearblue);
        }
        // Invisible
        else if(game.powerup[i].type==6){
            sprite.invisiblegreen.setPosition( game.powerup[i].x-config.powerup_radius , game.powerup[i].y-config.powerup_radius );
            sprite.invisiblegreen.setColor(color);
            window.draw(sprite.invisiblegreen);
        }
        // Walls Away
        else if(game.powerup[i].type==7){
            sprite.wallsawayblue.setPosition( game.powerup[i].x-config.powerup_radius , game.powerup[i].y-config.powerup_radius );
            sprite.wallsawayblue.setColor(color);
            window.draw(sprite.wallsawayblue);
        }
        // More
        else if(game.powerup[i].type==8){
            sprite.morepowerblue.setPosition( game.powerup[i].x-config.powerup_radius , game.powerup[i].y-config.powerup_radius );
            sprite.morepowerblue.setColor(color);
            window.draw(sprite.morepowerblue);
        }
        // Inverted
        else if(game.powerup[i].type==9){
            sprite.invertedred.setPosition( game.powerup[i].x-config.powerup_radius , game.powerup[i].y-config.powerup_radius );
            sprite.invertedred.setColor(color);
            window.draw(sprite.invertedred);
        }
    }
}
//
void Renderer::Game_Status_Menu(const Config &config,Game &game, Player player[],Game_Store &game_store){
    //
    window.draw(game_store.scores);
    // Show Point and name
    for(int i=0;i<MAX_PLAYERS;i++){
        if(!player[i].enabled){continue;}
        // Name
        game_store.names[i].setString(player[i].name);
        window.draw(game_store.names[i]);
        // Score
        game_store.score[i].setString(int2string(player[i].points));
        window.draw(game_store.score[i]);
    }
    // Ronde Display
    window.draw(game_store.round[0]);
    game_store.round[1].setString(int2string(game.round));
    window.draw(game_store.round[1]);
    // Frame
    window.draw(game_store.frame[0]);
    game_store.frame[1].setString(int2string(game.frame));
    window.draw(game_store.frame[1]);
    // Fps
    window.draw(game_store.fps[0]);
    // Calculate
    /*int fps;
    if(game.frame % 30 == 0){
        sf::Time elapsed = game.fps_clock.restart();
        fps=int ( 30/ (elapsed.asSeconds()) );
        game.last_fps=fps;
    }
    else{
        fps=game.last_fps;
    }
    game_store.fps[1].setString(int2string(fps));*/
    if(game.frame % 30 == 0){
        sf::Time elapsed = game.fps_clock.restart();
        game.last_fps=int ( 30/ (elapsed.asSeconds()) );
    }
    game_store.fps[1].setString(int2string(game.last_fps));
    window.draw(game_store.fps[1]);
    // Packet
    if(game.server[1]||game.client[1]){
        window.draw(game_store.packet[0]);
        game_store.packet[1].setString(int2string(1.0/game.packettime));
        window.draw(game_store.packet[1]);
    }
    // Return button
    if(game_store.quit.Check(game,window)){
        game.mode=Game::Mode::Main_Menu;
        if(game.server[1]){
            game.server[2]=true;
        }
        else if(game.client[1]){
            game.client[2]=true;
        }
        if(game.update_thread[1]){
            game.update_thread[2]=true;
        }
    }
    window.draw(game_store.quit);
}
//

