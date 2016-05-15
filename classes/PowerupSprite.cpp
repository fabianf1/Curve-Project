// Contains functions and constructors for the powerup sprite class
// Needed Header
#include "PowerupSprite.h"
// Constructor
PowerupSprite::PowerupSprite(const Config &config){
    sf::Image tempImage = Resource::loadImage("texture");
    texture.loadFromImage(tempImage);
    texture.setSmooth(true);
    // Fast sprites
    fastGreen.setTexture(texture);
    fastGreen.setTextureRect(sf::IntRect(0, 0, 100, 100));// Left, top, width, height
    fastGreen.setScale(config.powerupScale,config.powerupScale);
    fastRed.setTexture(texture);
    fastRed.setTextureRect(sf::IntRect(100, 0, 100, 100));
    fastRed.setScale(config.powerupScale,config.powerupScale);
    // Slow Sprites
    slowGreen.setTexture(texture);
    slowGreen.setTextureRect(sf::IntRect(200, 0, 100, 100));
    slowGreen.setScale(config.powerupScale,config.powerupScale);
    slowRed.setTexture(texture);
    slowRed.setTextureRect(sf::IntRect(300, 0, 100, 100));
    slowRed.setScale(config.powerupScale,config.powerupScale);
    // Small Sprites
    smallGreen.setTexture(texture);
    smallGreen.setTextureRect(sf::IntRect(0, 100, 100, 100));
    smallGreen.setScale(config.powerupScale,config.powerupScale);
    smallRed.setTexture(texture);
    smallRed.setTextureRect(sf::IntRect(100, 100, 100, 100));
    smallRed.setScale(config.powerupScale,config.powerupScale);
    // big Sprites
    bigGreen.setTexture(texture);
    bigGreen.setTextureRect(sf::IntRect(200, 100, 100, 100));
    bigGreen.setScale(config.powerupScale,config.powerupScale);
    bigRed.setTexture(texture);
    bigRed.setTextureRect(sf::IntRect(300, 100, 100, 100));
    bigRed.setScale(config.powerupScale,config.powerupScale);
    // Right Angle Sprites
    angleGreen.setTexture(texture);
    angleGreen.setTextureRect(sf::IntRect(0, 200, 100, 100));
    angleGreen.setScale(config.powerupScale,config.powerupScale);
    angleRed.setTexture(texture);
    angleRed.setTextureRect(sf::IntRect(100, 200, 100, 100));
    angleRed.setScale(config.powerupScale,config.powerupScale);
    // Clear Sprite
    clearBlue.setTexture(texture);
    clearBlue.setTextureRect(sf::IntRect(200, 200, 100, 100));
    clearBlue.setScale(config.powerupScale,config.powerupScale);
    // Invisible Sprite
    invisibleGreen.setTexture(texture);
    invisibleGreen.setTextureRect(sf::IntRect(300, 200, 100, 100));
    invisibleGreen.setScale(config.powerupScale,config.powerupScale);
    // no walls
    wallsAwayBlue.setTexture(texture);
    wallsAwayBlue.setTextureRect(sf::IntRect(0, 300, 100, 100));
    wallsAwayBlue.setScale(config.powerupScale,config.powerupScale);
    // More powerups
    morePowerupsBlue.setTexture(texture);
    morePowerupsBlue.setTextureRect(sf::IntRect(100, 300, 100, 100));
    morePowerupsBlue.setScale(config.powerupScale,config.powerupScale);
    // inverted Red
    invertedRed.setTexture(texture);
    invertedRed.setTextureRect(sf::IntRect(200, 300, 100, 100));
    invertedRed.setScale(config.powerupScale,config.powerupScale);
    // Question Mark Blue
    questionMarkBlue.setTexture(texture);
    questionMarkBlue.setTextureRect(sf::IntRect(300, 300, 100, 100));
    questionMarkBlue.setScale(config.powerupScale,config.powerupScale);
    // Darkness Blue
    darknessBlue.setTexture(texture);
    darknessBlue.setTextureRect(sf::IntRect(0, 400, 100, 100));
    darknessBlue.setScale(config.powerupScale,config.powerupScale);
    // Gap Red
    gapRed.setTexture(texture);
    gapRed.setTextureRect(sf::IntRect(100, 400, 100, 100));
    gapRed.setScale(config.powerupScale,config.powerupScale);
    // Bomb Blue
    bombBlue.setTexture(texture);
    bombBlue.setTextureRect(sf::IntRect(200, 400, 100, 100));
    bombBlue.setScale(config.powerupScale,config.powerupScale);
    // Sine Red
    sineRed.setTexture(texture);
    sineRed.setTextureRect(sf::IntRect(300, 400, 100, 100));
    sineRed.setScale(config.powerupScale,config.powerupScale);
    // Glitch Blue
    glitchBlue.setTexture(texture);
    glitchBlue.setTextureRect(sf::IntRect(0, 500, 100, 100));
    glitchBlue.setScale(config.powerupScale,config.powerupScale);
    // Radius Blue
    radiusBlue.setTexture(texture);
    radiusBlue.setTextureRect(sf::IntRect(100, 500, 100, 100));
    radiusBlue.setScale(config.powerupScale,config.powerupScale);
    // Glitch Red
    glitchRed.setTexture(texture);
    glitchRed.setTextureRect(sf::IntRect(300, 500, 100, 100));
    glitchRed.setScale(config.powerupScale,config.powerupScale);
    // No Turtle Red
    noTurtleRed.setTexture(texture);
    noTurtleRed.setTextureRect(sf::IntRect(0, 600, 100, 100));
    noTurtleRed.setScale(config.powerupScale,config.powerupScale);
    // No Turtle Blue
    noTurtleBlue.setTexture(texture);
    noTurtleBlue.setTextureRect(sf::IntRect(100, 600, 100, 100));
    noTurtleBlue.setScale(config.powerupScale,config.powerupScale);
    // Sine Green
    sineGreen.setTexture(texture);
    sineGreen.setTextureRect(sf::IntRect(200, 600, 100, 100));
    sineGreen.setScale(config.powerupScale,config.powerupScale);
    // No Turtle Blue
    multiplierGreen.setTexture(texture);
    multiplierGreen.setTextureRect(sf::IntRect(300, 600, 100, 100));
    multiplierGreen.setScale(config.powerupScale,config.powerupScale);
}
