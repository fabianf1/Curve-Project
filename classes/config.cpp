// Contains functions and constructors for the config class
// Header
#include "Config.h"
// Constructor
Config::Config(){
    title="Curve Project";
    // Window Settings
    windowWidth=1000;
    windowHeight=600;
    windowStyle=sf::Style::Close;
    windowAntialiasing=8;
    statusWidth=200;
    windowBackgroundColor=sf::Color::Black;
    fps=60;
    maxPlayers=6;
    // Icon
    icon = LoadImageFromResource("curveicon");
    // Line Settings
    lineWidth=8;
    //
    wallWidth=8;
    safeHeading=70;
    safeSpawn=5; // Extra separation between wall and line at start
    wallColor=sf::Color::White;
    // Speed Settings
    #ifdef DEBUG
    shift=50;
    #else
    shift=75;
    #endif // DEBUG
    turn=shift*13/8;
    // Powerup Settings
    // Slow
    slowScaling=0.5; // How fast it will go to the minimum speed;
    slowMinMultiplier=0.01; // The minimum multiplier it can reach
    slowTurnScaling=0.25;
    slowTurnMinMultiplier=0.01;
    // Fast
    fastScaling=0.4;
    fastMaxMultiplier=6;
    fastTurnScaling=0.275; // At faster speeds turning will also go slower(if fastTurnScaling<fastScaling)
    fastTurnMaxMultiplier=6;
    // Line size
    smalMultiplier=0.6;
    bigMultiplier=1.75;
    //
    darknessRadius=100;
    bombRadius=150;
    //
    sineFrequency=0.5; // T=2sec;
    sineAmplitude=0.6;
    //
    glitchMinWait=0.1;// Seconds;
    glitchMaxWait=0.5;
    glitchMoveChance=20; // For left and right separately
    glitchMaxMove=20;
    glitchWidthChance=30;
    glitchMaxWidthIncrease=1;
    glitchMaxWidthDecrease=0.5;
    glitchSpeedChance=30;
    glitchMaxSpeedIncrease=1;
    glitchMaxSpeedDecrease=0.5;
    //
    radiusMinScale=0.01;
    radiusScaleFactor= 0.0075 ; // Per unit of length. Additive;
    //
    powerupSpawnChance=10; //5;// % per powerupSpawnCheck second
    powerupSpawnCheck=0.5;
    powerupScale=0.4; // Textures are 100x100
    powerupRadius=40/2; // 40 is full width
    powerupMinDisappear=15.0;
    powerupRandDisappear=15000;
    powerupMorePowerupDelay=2.0;
    powerupSpawnDelay=1; //2.6;
    powerupFadeTime=2.0; // Time it takes for a powerup to fade in or out
    powerupSafeDistance=shift*2;
    powerupSafeRadius=shift*2;
    // Gap Settings
    minToGap=5.0; // In seconds
    randToGap=10000; // In milliseconds
    minToGapPowerup=0.05; // In seconds
    rand_to_gap_powerup=500; // In milliseconds
    minWidthGap=0.2;
    randWidthGap=300;
    //
    mainThreadMinTime=1000/30;
    gameUpdateThreadMinTime=1000/30;
    maxElapsed=0.1;
    //
    port=55003;
    maxAttempts=3;
    attemptDelay=100;
    lagTime=1.0;
    //
    version="0.3";
}
