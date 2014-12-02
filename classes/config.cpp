// Contains functions and constructors for the config class
// Header
#include "config.h"
// Temporary
#include "../functions/functions.h"
// Constructor
Config::Config(){
    title="Curve Project";
    // Window Settings
    window_width=1000;
    window_height=600;
    window_style=sf::Style::Close;
    window_antialising=8;
    statuswidth=200;
    window_backgroundcolor=sf::Color::Black;
    fps=60;
    // Icon
    icon = LoadImageFromResource("curveicon");
    // Font Settings
    font=LoadFont("fontdata");
//    if(!font.loadFromFile("arial.ttf")){
//        std::cout << "Font Error";
//    }
    font_title_size=50;
    font_size=30;
    // Line Settings
    linewidth=8;
    linecolors[0]=sf::Color::Red;
    linecolors[1]=sf::Color::Yellow;
    linecolors[2]=sf::Color::White;
    linecolors[3]=sf::Color::Green;
    linecolors[4]=sf::Color::Magenta;
    linecolors[5]=sf::Color::Blue;
    //
    wallwidth=8;
    safeheading=70;
    safespawn=5; // Extra separation between wall and line at start
    wallcolor=sf::Color::White;
    // Speed Settings
    shift=75; // Per second
    turn=shift*3/2;
    // Powerup Settings
    slow_multiplier=0.75;
    fast_multiplier=1.5;
    small_multiplier=0.6;
    big_multiplier=1.75;
    powerup_spawn_chance=5;// % per 0.5 second
    powerup_spawn_check=0.5;
    powerup_scale=0.4; // Textures are 100x100
    powerup_radius=40/2; // 40 is full width
    powerup_min_disappear=15.0;
    powerup_rand_disappear=15000;
    powerup_effect_min_disappear=5;
    powerup_effect_rand_disappear=7500;
    powerup_more_powerup_delay=2.0;
    powerup_spawn_delay=2.6;
    powerup_fade_time=2.0;
    powerup_safe_d=shift;
    powerup_safe_radius=powerup_radius+30;
    // Gap Settings
    min_to_gap=5.0;
    rand_to_gap=10000;
    min_width_gap=0.2;
    rand_width_gap=300;
    //
    main_thread_min_time=1000/30;
    game_update_thread_min_time=1000/30;
    max_dt=0.1;
    //
    port=55003;
    max_attempts=3;
    attempt_delay=500;
    lagtime=1.5;
}
