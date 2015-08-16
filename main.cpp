// Headers
#include "classes/main.h"

// Main Loop
int main(){
    // Initialize class and start main loop
    Main main;
    main.curveProject();
    // Wait a little so it is possible to read the console
    #ifdef DEBUG
        sf::sleep(sf::milliseconds(2000));
    #endif // DEBUG
    // Done
    return EXIT_SUCCESS;
}
