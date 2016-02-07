// Defines the object storage classes
#ifndef CURVE_OBJECT
#define CURVE_OBJECT
// Standard Headers
#include <SFML/Graphics.hpp>
#include "ObjectBase.h"
#include "player.h"
// Forward declaration
class Main;
//
class ObjectLine : public ObjectBase{
public:
    bool checkVisible( Main &main );
    void draw (sf::RenderWindow &window ); // Will need more info for darkness; Save pointer to player?
    void drawDarkness( sf::RenderWindow &window, sf::VertexArray &line );
    sf::VertexArray *line;
    sf::VertexArray *noTurtleLine;
    unsigned int id;
    std::vector<Player> *player;
    bool *darkness;
    const Config *config;
    ObjectLine( sf::VertexArray *i_line, sf::VertexArray *i_noTurtleLine, unsigned int &i_id, std::vector<Player> *i_player, bool *i_darkness, const Config &i_config );
};

#endif // CURVE_OBJECT
