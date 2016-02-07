// Contains functions and constructors for the Renderer class
// Needed Header
#include "renderer.h"
//
void Renderer::start(Main &main, const Config &config,Game &game,std::vector<std::unique_ptr<ObjectBase>> &objects, const std::vector<Player> &player){
    // Create and initialize render window
    std::cout << "Initializing window" << std::endl;
    window.create(sf::VideoMode(config.windowWidth, config.windowHeight), config.title, config.windowStyle, sf::ContextSettings(24,8,config.windowAntialiasing));
    window.setIcon(202,202,config.icon.getPixelsPtr());
    window.clear(config.windowBackgroundColor);
    window.setFramerateLimit(config.fps);
    window.display();
    // Start the render thread
    std::cout << "Starting render thread" << std::endl;
    window.setActive(false);
    renderThread = std::thread(&Renderer::thread,this,std::ref(main),std::cref(config),std::ref(game),std::ref(objects), std::cref(player));
}
// Render thread Function
void Renderer::thread(Main &main, const Config &config,Game &game,std::vector<std::unique_ptr<ObjectBase>> &objects, const std::vector<Player> &player){
    std::cout << "Render thread Started" << std::endl;
    // Start main loop
    while(window.isOpen()){
        window.clear(config.windowBackgroundColor);
        // Locking the mutex when changing modes solves font rendering problems
        game.modeMutex.lock();
        // Loop through objects
        for(unsigned int i=0; i<objects.size();i++){
            if(objects[i]->checkVisible(main)){
                objects[i]->draw(window);
            }
        }
        game.modeMutex.unlock();
        // Display everything to window and update fps
        window.display();
        game.frame++;
        if(game.frame%(config.fps)==0){
            fps =(1.0/game.fpsClock.restart().asSeconds())*(config.fps);
        }
    }
    std::cout << "Render thread Stopped" << std::endl;
}
//
void Renderer::shutdown(){
    window.close();
    if(renderThread.joinable()){
        renderThread.join();
    }
}
//
