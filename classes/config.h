// Contains config class declaration
#ifndef CURVE_CONFIG
#define CURVE_CONFIG
// SFML Headers
#include <SFML/Graphics.hpp>
// For LoadImageFromResource
#include "../functions/functions.h"
//
class Config{
    public:
        // Vars
        sf::String title; // Title of the game;
        //
        int window_width,window_height; // Height and width of screen
        int window_style; // Style of window
        int window_antialising; // Declare antialiasing level
        int statuswidth;
        sf::Color window_backgroundcolor;
        sf::Image icon;
        //
        int fps; // Maximum number of fps
        //
        int linewidth;
        sf::Color linecolors[6];
        //
        int wallwidth;
        int safeheading;
        int safespawn;
        sf::Color wallcolor;
        //
        int shift;
        int turn;
        //Powerup Things
        float slow_scaling;
        float slow_min_multiplier;
        float slow_turn_scaling;
        float slow_turn_min_multiplier;
        //
        float fast_scaling;
        float fast_max_multiplier;
        float fast_turn_scaling;
        float fast_turn_max_multiplier;
        float small_multiplier;
        float big_multiplier;
        float powerup_scale;
        float powerup_radius;
        int powerup_spawn_chance;
        float powerup_spawn_check;
        float powerup_min_disappear; // Min time for dissapearance
        int powerup_rand_disappear; // Random time for disappear in ms;
        float powerup_effect_min_disappear;
        int powerup_effect_rand_disappear;
        float powerup_more_powerup_delay;
        float powerup_spawn_delay; // Delay for spawning after round start
        float powerup_fade_time;
        int powerup_safe_d;
        int powerup_safe_radius;
        //
        float min_to_gap; // seconds
        int rand_to_gap; // ms
        float min_width_gap;
        int rand_width_gap;
        //
        int main_thread_min_time;
        int game_update_thread_min_time;
        float max_dt; // Maximum game update time difference;
        //
        int port;
        int max_attempts;
        int attempt_delay;
        float lagtime;
        // Constructor
        Config();
};
//
#endif // CURVE_CONFIG
