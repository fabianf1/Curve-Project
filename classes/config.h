// Contains config class declaration
#ifndef CURVE_CONFIG
#define CURVE_CONFIG
// Headers
#include <string>
#include <SFML/Graphics.hpp>
#include "../functions/General.h"
//
class Config{
    public:
        // Vars
        sf::String title; // Title of the game;
        //
        int windowWidth,windowHeight; // Height and width of screen
        int windowStyle; // Style of window
        int windowAntialiasing; // Declare antialiasing level
        int statusWidth;
        sf::Color windowBackgroundColor;
        sf::Image icon;
        //
        int fps; // Maximum number of fps
        unsigned int maxPlayers;
        //
        int lineWidth;
        //
        int wallWidth;
        int safeHeading;
        int safeSpawn;
        sf::Color wallColor;
        //
        int shift;
        int turn;
        //Powerup Things
        float slowScaling;
        float slowMinMultiplier;
        float slowTurnScaling;
        float slowTurnMinMultiplier;
        //
        float fastScaling;
        float fastMaxMultiplier;
        float fastTurnScaling;
        float fastTurnMaxMultiplier;
        //
        float smalMultiplier;
        float bigMultiplier;
        //
        int darknessRadius;
        int bombRadius;
        //
        float sineFrequency;
        float sineAmplitude;
        //
        float glitchMinWait;
        float glitchMaxWait;
        int glitchMoveChance;
        int glitchMaxMove;
        int glitchWidthChance;
        float glitchMaxWidthIncrease;
        float glitchMaxWidthDecrease;
        int glitchSpeedChance;
        float glitchMaxSpeedIncrease;
        float glitchMaxSpeedDecrease;
        //
        float radiusMinScale;
        float radiusScaleFactor;
        //
        float powerupScale;
        int powerupRadius;
        int powerupSpawnChance;
        float powerupSpawnCheck;
        float powerupMinDisappear; // Min time for dissapearance
        int powerupRandDisappear; // Random time for disappear in ms;
        float powerupMorePowerupDelay;
        float powerupSpawnDelay; // Delay for spawning after round start
        float powerupFadeTime;
        int powerupSafeDistance; // Distance in front of player that should be safe
        int powerupSafeRadius;
        //
        float minToGap; // seconds
        int randToGap; // ms
        float minToGapPowerup; // seconds
        int rand_to_gap_powerup; // ms
        float minWidthGap;
        int randWidthGap;
        //
        int mainThreadMinTime;
        int gameUpdateThreadMinTime;
        float maxElapsed; // Maximum game update time difference;
        //
        int port;
        int maxAttempts;
        int attemptDelay;
        float lagTime;
        //
        std::string version;
        // Constructor
        Config();
};
//
#endif // CURVE_CONFIG
