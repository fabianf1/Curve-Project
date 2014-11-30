// Headers
#include "classes/main.h"

// Main Loop
int main(){
    // Initialize class
    Main main;
    main.Curve_Project();
    #ifdef DEBUG
        sf::sleep(sf::milliseconds(2000));
    #endif // DEBUG
    // Done
    return EXIT_SUCCESS;
}
