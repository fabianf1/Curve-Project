// Contains the main Loop
#include "main.h"

//
void Main::Curve_Project(){
    // Start initialization
    // Open Window
    window.create(sf::VideoMode(config.window_width, config.window_height), config.title, config.window_style, sf::ContextSettings(24,8,config.window_antialising));
    window.setIcon(202,202,config.icon.getPixelsPtr());
    window.clear(config.window_backgroundcolor);
    window.display();
    // Limit fps
    window.setFramerateLimit(config.fps);
    // Start Render thread
    window.setActive(false);
    game.threads.emplace_back(Render_Thread,std::cref(config),std::ref(game),player,std::ref(window));
    // Main loop
    while(window.isOpen()){
        // Event Handler
        sf::Event event;
        while (window.pollEvent(event)){
            Event_Handler(config,game,player,window,event);
        }
        // Other things too do
        if(game.update_thread[0]&&!game.update_thread[1]){
            Start_Game_Update(config,game,player);
        }
        else if(game.server[0]&&!game.server[1]){
            server.Start_Server(config,game,player);
        }
        else if(game.client[0]&&!game.client[1]){
            Start_Client(config,game,player);
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
    // Server
    server.Shutdown(); // Need to look here later
    std::cout << "All Threads Finished!" << std::endl;
}
