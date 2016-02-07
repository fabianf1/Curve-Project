// Defines the object storage classes
#ifndef CURVE_OBJECTPOWERUPS
#define CURVE_OBJECTPOWERUPS
// Standard Headers
#include <SFML/Graphics.hpp>
#include "ObjectBase.h"
#include "player.h"
#include "PowerupSprite.h"
#include "PowerupField.h"
//
class ObjectPowerups : public ObjectBase{
public:
    bool checkVisible(Main &main);
    void draw(sf::RenderWindow &window);
    std::vector<PowerupField> *powerups;
    std::vector<Player> *player;
    bool *darkness;
    PowerupSprite sprite;
    const Config *config;
    ObjectPowerups( const Config &i_config, std::vector<PowerupField> *i_powerups, std::vector<Player> *i_player, bool *i_darkness );
};
//

#endif // CURVE_OBJECTPOWERUPS

