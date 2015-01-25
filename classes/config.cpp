// Contains functions and constructors for the config class
// Header
#include "config.h"
// Constructor
Config::Config(){
    title="Curve Project";
    // Window Settings
    window_width=1000;
    window_height=600;
    window_style=sf::Style::Close;
    window_antialiasing=8;
    statuswidth=200;
    window_backgroundcolor=sf::Color::Black;
    fps=60;
    max_players=6;
    // Icon
    icon = LoadImageFromResource("curveicon");
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
    shift=50; //75; // Per second
    turn=shift*13/8;
    // Powerup Settings
    // Slow
    slow_scaling=0.5; // How fast it will go to the minimum speed;
    slow_min_multiplier=0.01; // The minimum multiplier it can reach
    slow_turn_scaling=0.25;
    slow_turn_min_multiplier=0.01;
    // Fast
    fast_scaling=0.4;
    fast_max_multiplier=6;
    fast_turn_scaling=0.275; // At faster speeds turning will also go slower(if fast_turn_scaling<fast_scaling)
    fast_turn_max_multiplier=6;
    // Line size
    small_multiplier=0.6;
    big_multiplier=1.75;
    //
    darkness_radius=125;
    bomb_radius=250;
    //
    sine_frequency=0.5; // T=2sec;
    sine_amplitude=0.6;
    //
    powerup_spawn_chance=10; //5;// % per powerup_spawn_check second
    powerup_spawn_check=0.5;
    powerup_scale=0.4; // Textures are 100x100
    powerup_radius=40/2; // 40 is full width
    powerup_min_disappear=15.0;
    powerup_rand_disappear=15000;
    powerup_effect_min_disappear=10.0; // In seconds
    powerup_effect_rand_disappear=5000; // In milliseconds
    powerup_more_powerup_delay=2.0;
    powerup_spawn_delay=1; //2.6;
    powerup_fade_time=2.0; // Time it takes for a powerup to fade in or out
    powerup_safe_d=shift;
    powerup_safe_radius=powerup_radius*5/2;
    // Gap Settings
    min_to_gap=5.0; // In seconds
    rand_to_gap=10000; // In milliseconds
    min_to_gap_powerup=0.05; // In seconds
    rand_to_gap_powerup=500; // In milliseconds
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
    lagtime=1.0;
}
