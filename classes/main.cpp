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
        // Other things too do
        if(game.update_thread[0]&&!game.update_thread[1]){
            Start_Game_Update(config,game,player);
        }
        else if(game.server[0]&&!game.server[1]){
            server.Start(config,game,player);
        }
        else if(game.client[0]&&!game.client[1]){

            client.Start(config,game,player);
        }
        // Delay function
        Sleep(config.main_thread_min_time,game.main_thread_clock);
    }
    // Shutdown procedure
    game.update_thread[0]=false;
    game.server[0]=false;
    game.client[0]=false;
    game.update_thread[2]=true;
    game.server[2]=true;
    game.client[2]=true;
    for(unsigned int i=0;i<game.threads.size();i++){
        game.threads[i].join();
    }
    server.Shutdown(); // Need to look here later
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
        Pause_Game(config,game,true);
    }
    // Regain Focus
    /*else if(event.type == sf::Event::GainedFocus){
        Pause_Game(config,game,window,false);
    }*/
    // Client connect
    else if(game.mode==Game::Mode::Server_IP){
        if(event.type==sf::Event::TextEntered&&( (event.text.unicode>=48&&event.text.unicode<=57)||event.text.unicode==46) &&game.server_ip.getSize()<16){
            game.server_ip+=static_cast<char>(event.text.unicode);
        }
        else if(event.type==sf::Event::KeyPressed&&event.key.code == sf::Keyboard::BackSpace&&game.server_ip.getSize()>0){
            game.server_ip.erase(game.server_ip.getSize()-1,1);
        }
    }
    // Setup screen SP
    else if(game.mode==Game::Mode::Setup_SP){
        // Key changer
        if(game.keychange[0]!=-1){
            if(event.type==sf::Event::KeyPressed){
                if(event.key.code==sf::Keyboard::Escape){
                    game.keychange[0]=-1;
                    game.player_switched=false;
                }
                else if(game.keychange[1]==1){
                    // Check availability
                    if(Key_Available(player,event.key.code)){
                        player[game.keychange[0]].keyL=event.key.code;
                        if(game.player_switched){
                            game.keychange[1]=2;
                        }
                        else{
                            game.keychange[0]=-1;
                        }
                    }
                }
                else if(game.keychange[1]==2){
                    if(Key_Available(player,event.key.code)){
                        player[game.keychange[0]].keyR=event.key.code;
                        game.keychange[0]=-1;
                        game.player_switched=false;
                    }
                }
            }
        }
        #ifdef DEBUG
        // Auto adder, only in debug
        else if(event.type==sf::Event::KeyPressed&&event.key.code==sf::Keyboard::Space){
            Auto_Add_Players(player);
        }
        #endif
    }
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
        // KeyChange
        else if(game.keychange[1]==1&&event.type==sf::Event::KeyPressed&&event.key.code!=sf::Keyboard::Space&&event.key.code!=player[game.id].keyR){
            player[game.id].keyL=event.key.code;
            game.keychange[1]=-1;
        }
        else if(game.keychange[1]==2&&event.type==sf::Event::KeyPressed&&event.key.code!=sf::Keyboard::Space&&event.key.code!=player[game.id].keyL){
            player[game.id].keyR=event.key.code;
            game.keychange[1]=-1;
        }
        // End Keychange
        // Debug
        #ifdef DEBUG
        else if(event.type==sf::Event::KeyPressed&&event.key.code==sf::Keyboard::Space){
            if(game.server[1]){
                player[0].keyL=sf::Keyboard::Left;
                player[0].keyR=sf::Keyboard::Right;
            }
            else if(game.client[1]){
                player[game.id].keyL=sf::Keyboard::Z;
                player[game.id].keyR=sf::Keyboard::X;
            }
        }
        #endif // DEBUG
    }
    // Game mode
    else if(game.mode==Game::Mode::Play){
        // End game
        if(game.game_finished&&event.type==sf::Event::KeyPressed&&event.key.code==sf::Keyboard::Space){
            game.mode=Game::Mode::Setup_SP;
        }
        // New Round
        else if(game.round_finished&&event.type==sf::Event::KeyPressed&&event.key.code==sf::Keyboard::Space){
            Initialize_New_Round(config,game,player);
        }
        // Pause
        else if(event.type==sf::Event::KeyPressed&&event.key.code==sf::Keyboard::Space){
            if(game.pause){Pause_Game(config,game,false);}
            else{Pause_Game(config,game,true);}
        }
    }
    //
    else if(game.mode==Game::Mode::Play_MP){
        // End game
        if(game.game_finished&&event.type==sf::Event::KeyPressed&&event.key.code==sf::Keyboard::Space){
            game.mode=Game::Mode::Setup_MP;
            // Force Sync as server
            if(game.server[1]){
                server.Sync_Clients(config,game,player);
            }
        }
        // New Round
        else if(game.round_finished&&event.type==sf::Event::KeyPressed&&event.key.code==sf::Keyboard::Space&&game.server[1]){
            Initialize_New_Round_Server(config,game,player);
        }
        // Pause
        #ifdef DEBUG
        else if(event.type==sf::Event::KeyPressed&&event.key.code==sf::Keyboard::Space&&game.server[1]){
            if(game.pause){Pause_Game(config,game,false);}
            else{Pause_Game(config,game,true);}
        }
        #endif // DEBUG
    }
    //
}
//
