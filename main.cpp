// Headers
#include "curve.h"

// Main Loop
int main(){
    // Start initialization
    // Create Config Variable
    Config config;
    // Open Window
    sf::RenderWindow window(sf::VideoMode(config.window_width, config.window_height), config.title, config.window_style, sf::ContextSettings(24,8,config.window_antialising));
    window.setIcon(202,202,config.icon.getPixelsPtr());
    window.clear(config.window_backgroundcolor);
    window.display();
    // Limit fps
    window.setFramerateLimit(config.fps);
    // Init some vars
    Game game;
    Player player[MAX_PLAYERS];
    Server server;
    // Start Render thread
    window.setActive(false);
    game.threads.emplace_back(Render_Thread,std::cref(config),std::ref(game),player,std::ref(window));
    // Equivalent=game.threads.push_back(std::thread (Render_Thread,std::cref(config),std::ref(game),player,std::ref(window))); But now without copying
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
            Start_Server(config,game,player,server);
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
    std::cout << "All Threads Finished!" << std::endl;

    #ifdef DEBUG
        sf::sleep(sf::milliseconds(2000));
    #endif // DEBUG

    // Done
    return EXIT_SUCCESS;
}
