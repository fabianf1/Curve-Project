// Contains functions and constructors for the powerup sprite class
// Needed Header
#include "powerup_sprite.h"
// Constructor
Powerup_Sprite::Powerup_Sprite(const Config &config){
    sf::Image tempImage = LoadImageFromResource("texture");
    texture.loadFromImage(tempImage);
    texture.setSmooth(true);
    // Fast sprites
    fastgreen.setTexture(texture);
    fastgreen.setTextureRect(sf::IntRect(0, 0, 100, 100));// Left, top, width, height
    fastgreen.setScale(config.powerup_scale,config.powerup_scale);
    fastred.setTexture(texture);
    fastred.setTextureRect(sf::IntRect(100, 0, 100, 100));
    fastred.setScale(config.powerup_scale,config.powerup_scale);
    // Slow Sprites
    slowgreen.setTexture(texture);
    slowgreen.setTextureRect(sf::IntRect(200, 0, 100, 100));
    slowgreen.setScale(config.powerup_scale,config.powerup_scale);
    slowred.setTexture(texture);
    slowred.setTextureRect(sf::IntRect(300, 0, 100, 100));
    slowred.setScale(config.powerup_scale,config.powerup_scale);
    // Small Sprites
    smallgreen.setTexture(texture);
    smallgreen.setTextureRect(sf::IntRect(0, 100, 100, 100));
    smallgreen.setScale(config.powerup_scale,config.powerup_scale);
    smallred.setTexture(texture);
    smallred.setTextureRect(sf::IntRect(100, 100, 100, 100));
    smallred.setScale(config.powerup_scale,config.powerup_scale);
    // big Sprites
    biggreen.setTexture(texture);
    biggreen.setTextureRect(sf::IntRect(200, 100, 100, 100));
    biggreen.setScale(config.powerup_scale,config.powerup_scale);
    bigred.setTexture(texture);
    bigred.setTextureRect(sf::IntRect(300, 100, 100, 100));
    bigred.setScale(config.powerup_scale,config.powerup_scale);
    // Right Angle Sprites
    anglegreen.setTexture(texture);
    anglegreen.setTextureRect(sf::IntRect(0, 200, 100, 100));
    anglegreen.setScale(config.powerup_scale,config.powerup_scale);
    anglered.setTexture(texture);
    anglered.setTextureRect(sf::IntRect(100, 200, 100, 100));
    anglered.setScale(config.powerup_scale,config.powerup_scale);
    // Clear Sprite
    clearblue.setTexture(texture);
    clearblue.setTextureRect(sf::IntRect(200, 200, 100, 100));
    clearblue.setScale(config.powerup_scale,config.powerup_scale);
    // Invisible Sprite
    invisiblegreen.setTexture(texture);
    invisiblegreen.setTextureRect(sf::IntRect(300, 200, 100, 100));
    invisiblegreen.setScale(config.powerup_scale,config.powerup_scale);
    // no walls
    wallsawayblue.setTexture(texture);
    wallsawayblue.setTextureRect(sf::IntRect(0, 300, 100, 100));
    wallsawayblue.setScale(config.powerup_scale,config.powerup_scale);
    // More powerups
    morepowerblue.setTexture(texture);
    morepowerblue.setTextureRect(sf::IntRect(100, 300, 100, 100));
    morepowerblue.setScale(config.powerup_scale,config.powerup_scale);
    // inverted Red
    invertedred.setTexture(texture);
    invertedred.setTextureRect(sf::IntRect(200, 300, 100, 100));
    invertedred.setScale(config.powerup_scale,config.powerup_scale);
    // Question Mark Blue
    questionblue.setTexture(texture);
    questionblue.setTextureRect(sf::IntRect(300, 300, 100, 100));
    questionblue.setScale(config.powerup_scale,config.powerup_scale);
    // Darkness Blue
    darknessblue.setTexture(texture);
    darknessblue.setTextureRect(sf::IntRect(0, 400, 100, 100));
    darknessblue.setScale(config.powerup_scale,config.powerup_scale);
    // Gap Red
    gapred.setTexture(texture);
    gapred.setTextureRect(sf::IntRect(100, 400, 100, 100));
    gapred.setScale(config.powerup_scale,config.powerup_scale);
    // Bomb Blue
    bombblue.setTexture(texture);
    bombblue.setTextureRect(sf::IntRect(200, 400, 100, 100));
    bombblue.setScale(config.powerup_scale,config.powerup_scale);
    // Sine Red
    sinered.setTexture(texture);
    sinered.setTextureRect(sf::IntRect(300, 400, 100, 100));
    sinered.setScale(config.powerup_scale,config.powerup_scale);
}
