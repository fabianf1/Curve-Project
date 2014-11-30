// Header guard
#ifndef CURVE_HEADER
#define CURVE_HEADER

// Defines needed
#define MAX_PLAYERS 6
#define PI 3.14159265359

// Include the other headers
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <thread>
#include <iostream>
#include <vector>
// Classes
// Online classes
// Other Classes
#include "classes/config.h"
#include "classes/powerup.h"
#include "classes/powerup_effect.h"
#include "classes/pending.h"
#include "classes/game.h"
#include "classes/player.h"
#include "classes/client.h"
#include "classes/server.h"
#include "classes/enum.h"
// GUI classes
#include "classes/text_button.h"
//#include "classes/prompt.h"
// Render Storages
#include "classes/main_menu.h"
#include "classes/game_setup.h"
#include "classes/game_setup_mp.h"
#include "classes/game_store.h"
#include "classes/powerup_sprite.h"

//
#include "functions/functions.h"

#endif // CURVE_HEADER

