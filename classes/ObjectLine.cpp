// Headers
#include "ObjectLine.h"
#include "main.h"
// Line
bool ObjectLine::checkVisible( Main &main ){
    if(main.player.size()>id){
        return true;
    }
    return false;
}
void ObjectLine::draw( sf::RenderWindow &window ){
    if(*darkness){
        drawDarkness( window, *line );
        drawDarkness( window, *noTurtleLine );
    }
    else{
        window.draw( *line );
        window.draw( *noTurtleLine );
    }
}
//
void ObjectLine::drawDarkness( sf::RenderWindow &window, sf::VertexArray &line ){
    sf::VertexArray quad(sf::Quads, 4);
    int xc, yc;
    for( unsigned int j=0; j+3 < line.getVertexCount(); j=j+4 ){
        xc = ( line[j].position.x + line[j+1].position.x + line[j+2].position.x + line[j+3].position.x ) / 4;
        yc = ( line[j].position.y + line[j+1].position.y + line[j+2].position.y + line[j+3].position.y ) / 4;
        for( unsigned int k=0; k < player->size(); k++ ){
            // Calculate center
            if( ( xc - (*player)[k].x ) * ( xc - (*player)[k].x ) + ( yc - (*player)[k].y ) * ( yc-(*player)[k].y ) < ( config->darknessRadius*config->darknessRadius ) ){
                quad[0] = line[j];
                quad[1] = line[j+1];
                quad[2] = line[j+2];
                quad[3] = line[j+3];
                window.draw(quad);
                break;
            }
        }
    }
}
//
ObjectLine::ObjectLine( sf::VertexArray *i_line, sf::VertexArray *i_noTurtleLine, unsigned int &i_id, std::vector<Player> *i_player, bool *i_darkness, const Config &i_config ): config(&i_config){
    line = i_line;
    noTurtleLine = i_noTurtleLine;
    id = i_id;
    player = i_player;
    darkness = i_darkness;
}
