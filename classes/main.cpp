// Contains the main Loop
#include "main.h"

//
void Main::Curve_Project(){
    // Start Renderer
    renderer.Start(config,game,player);
    // Main loop
    while(renderer.window.isOpen()){
        // Event Handler
        sf::Event event;
        while (renderer.window.pollEvent(event)){
            Event_Handler(config,game,player,renderer.window,event);
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
