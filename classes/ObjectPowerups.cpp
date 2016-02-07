// Contains functions and constructors for Objects
// Needed Header
#include "ObjectPowerups.h"
#include "main.h"
bool ObjectPowerups::checkVisible(Main &main){
    return true;
}
//
void ObjectPowerups::draw(sf::RenderWindow &window){
    for(auto powerup2 = powerups->begin(); powerup2!=powerups->end(); powerup2++){
        // Copy; Should try to embed this in the line above
        PowerupField powerup=*powerup2;
        // If darkness powerup is enabled only draw powerups within FoV
        if(*darkness){
            bool draw=false;
            int diffx;
            int diffy;
            for(unsigned int j=0;j<player->size();j++){
                diffx=( (*player)[j].x-powerup.x);
                diffy=( (*player)[j].y-powerup.y);
                if( diffx*diffx+diffy*diffy < (config->darknessRadius*config->darknessRadius)/4 ){
                    draw=true;
                    break;
                }
            }
            if(!draw){
                continue;
            }
        }
        // Set opacity for fade in and out effects
        int opacity=255;
        if(powerup.time1<config->powerupFadeTime){
            opacity=(powerup.time1/config->powerupFadeTime)*255;
        }
        else if(powerup.time2<config->powerupFadeTime){
            opacity=(powerup.time2/config->powerupFadeTime)*255;
        }
        sf::Color color(255,255,255,opacity);
        // Set sprite to draw
        sf::Sprite *draw;
        if(powerup.type==Powerup::Type::Fast){
            if(powerup.impact==Powerup::Impact::Self){
                draw=&sprite.fastGreen;
            }
            else if(powerup.impact==Powerup::Impact::Other){
                draw=&sprite.fastRed;
            }
        }
        else if(powerup.type==Powerup::Type::Slow){
            if(powerup.impact==Powerup::Impact::Self){
                draw=&sprite.slowGreen;
            }
            else if(powerup.impact==Powerup::Impact::Other){
                draw=&sprite.slowRed;
            }
        }
        else if(powerup.type==Powerup::Type::Small){
            if(powerup.impact==Powerup::Impact::Self){
                draw=&sprite.smallGreen;
            }
            else if(powerup.impact==Powerup::Impact::Other){
                draw=&sprite.smallRed;
            }
        }
        else if(powerup.type==Powerup::Type::Big){
            if(powerup.impact==Powerup::Impact::Self){
                draw=&sprite.bigGreen;
            }
            else if(powerup.impact==Powerup::Impact::Other){
                draw=&sprite.bigRed;
            }
        }
        else if(powerup.type==Powerup::Type::RightAngle){
            if(powerup.impact==Powerup::Impact::Self){
                draw=&sprite.angleGreen;
            }
            else if(powerup.impact==Powerup::Impact::Other){
                draw=&sprite.angleRed;
            }
        }
        else if(powerup.type==Powerup::Type::Clear){
            draw=&sprite.clearBlue;
        }
        else if(powerup.type==Powerup::Type::Invisible){
            draw=&sprite.invisibleGreen;
        }
        else if(powerup.type==Powerup::Type::WallsAway){
            draw=&sprite.wallsAwayBlue;
        }
        else if(powerup.type==Powerup::Type::MorePowerups){
            draw=&sprite.morePowerupsBlue;
        }
        else if(powerup.type==Powerup::Type::InvertKeys){
            draw=&sprite.invertedRed;
        }
        else if(powerup.type==Powerup::Type::QuestionMark){
            draw=&sprite.questionMarkBlue;
        }
        else if(powerup.type==Powerup::Type::Darkness){
            draw=&sprite.darknessBlue;
        }
        else if(powerup.type==Powerup::Type::Gap){
            draw=&sprite.gapRed;
        }
        else if(powerup.type==Powerup::Type::Bomb){
            draw=&sprite.bombBlue;
        }
        else if(powerup.type==Powerup::Type::Sine){
            if(powerup.impact==Powerup::Impact::Self){
                draw=&sprite.sineGreen;
            }
            else if(powerup.impact==Powerup::Impact::Other){
                draw=&sprite.sineRed;
            }
        }
        else if(powerup.type==Powerup::Type::Glitch){
            if(powerup.impact==Powerup::Impact::All){
                draw=&sprite.glitchBlue;
            }
            else if(powerup.impact==Powerup::Impact::Other){
                draw=&sprite.glitchRed;
            }
        }
        else if(powerup.type==Powerup::Type::Radius){
            draw=&sprite.radiusBlue;
        }
        else if(powerup.type==Powerup::Type::NoTurtle){
             if(powerup.impact==Powerup::Impact::All){
                draw=&sprite.noTurtleBlue;
            }
            else if(powerup.impact==Powerup::Impact::Other){
                draw=&sprite.noTurtleRed;
            }
        }
        else if(powerup.type==Powerup::Type::Multiplier){
            draw=&sprite.multiplierGreen;
        }
        else{
            std::cout << "Error! No sprite!" << std::to_string( powerup.place ) << std::endl;
            draw=&sprite.questionMarkBlue;
        }
        // Set sprite and draw
        draw->setPosition( powerup.x-config->powerupRadius , powerup.y-config->powerupRadius );
        draw->setColor(color);
        window.draw(*draw);
    }
}
//
ObjectPowerups::ObjectPowerups( const Config &i_config, std::vector<PowerupField> *i_powerups, std::vector<Player> *i_player, bool *i_darkness ) : sprite(i_config), config(&i_config){
    powerups=i_powerups;
    player=i_player;
    darkness=i_darkness;
}
//
