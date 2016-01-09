// Contains functions and constructors for Objects
// Needed Header
#include "Object.h"
#include "main.h"
// Implementation of ObjectStatic
template <class Drawable>
void ObjectStatic<Drawable>::draw(sf::RenderWindow &window){
    window.draw(drawable);
}
template <class Drawable>
bool ObjectStatic<Drawable>::checkVisible(Main &main){
    return visible(main,*this);
}
template <class Drawable>
void ObjectStatic<Drawable>::process(Main &main){
    if(update){
        update(main,*this);
    }
}
template <class Drawable>
ObjectStatic<Drawable>::ObjectStatic(Drawable &&i_drawable, std::function<bool(Main&,ObjectStatic<Drawable>&)> &&i_visible){
    visible=i_visible;
    drawable=i_drawable;
}
template <class Drawable>
ObjectStatic<Drawable>::ObjectStatic(Drawable &&i_drawable, std::function<bool(Main&,ObjectStatic<Drawable>&)> &&i_visible,std::function<void(Main&,ObjectStatic<Drawable>&)> &&i_update){
    visible=i_visible;
    drawable=i_drawable;
    update=i_update;
}
template <class Drawable>
ObjectStatic<Drawable>::ObjectStatic(Drawable &&i_drawable, std::function<bool(Main&,ObjectStatic<Drawable>&)> &&i_visible,std::function<void(Main&,ObjectStatic<Drawable>&)> &&i_update, unsigned int &&i_updateInfo){
    visible=i_visible;
    drawable=i_drawable;
    update=i_update;
    updateInfo=i_updateInfo;
}
// Explicit instantiation
template class ObjectStatic<sf::Text>;
template class ObjectStatic<sf::RectangleShape>;


// Implemenentation of ObjectButton
template <class Drawable>
void ObjectButton<Drawable>::draw(sf::RenderWindow &window){
    window.draw(drawable);
}
template <class Drawable>
bool ObjectButton<Drawable>::checkVisible(Main &main){
    return visible(main,*this);
}
template <class Drawable>
void ObjectButton<Drawable>::process(Main &main){
    if(update){
        update(main,*this);
    }
    if(check(main.renderer.window)){
        action(main,*this);
    }
}
template <class Drawable>
bool ObjectButton<Drawable>::check(sf::RenderWindow &window){
    // check selected
    if(drawable.getGlobalBounds().contains(sf::Mouse::getPosition(window).x,sf::Mouse::getPosition(window).y)){
        drawable.setColor(selectedColor);
        if(!activated){
            drawable.setStyle(selectedStyle);
        }
        else{
            drawable.setStyle(selectedStyle|activatedStyle);
        }
        if(window.hasFocus()&&sf::Mouse::isButtonPressed(sf::Mouse::Left) && lastClick.getElapsedTime().asSeconds()>0.15){
            lastClick.restart();
            return true;
        }
    }
    else{
        drawable.setColor(normalColor);
        if(!activated){
            drawable.setStyle(normalStyle);
        }
        else{
            drawable.setStyle(normalStyle|activatedStyle);
        }
    }
    return false;
}
// Constructor
template <class Drawable>
ObjectButton<Drawable>::ObjectButton(Drawable &&i_drawable, std::function<bool(Main&,ObjectButton<Drawable>&)> &&i_visible, std::function<void(Main&,ObjectButton<Drawable>&)> &&i_action,const sf::Color &i_selectedColor,const unsigned int &i_selectedStyle){
    action=i_action;
    visible=i_visible;
    drawable=i_drawable;
    selectedColor=i_selectedColor;
    selectedStyle=i_selectedStyle;
    normalColor=drawable.getColor();
    normalStyle=drawable.getStyle();
    activated=false;
    activatedStyle=sf::Text::Bold|sf::Text::Underlined;
}
template <class Drawable>
ObjectButton<Drawable>::ObjectButton(Drawable &&i_drawable, std::function<bool(Main&,ObjectButton<Drawable>&)> &&i_visible, std::function<void(Main&,ObjectButton<Drawable>&)> &&i_action, std::function<void(Main&,ObjectButton<Drawable>&)> &&i_update, unsigned int &i_updateInfo, const sf::Color &i_selectedColor,const unsigned int &i_selectedStyle):
    ObjectButton(std::move(i_drawable), std::move(i_visible), std::move(i_action), i_selectedColor,i_selectedStyle){
    update=i_update;
    updateInfo=i_updateInfo;
}
// Explicit instantiation
template class ObjectButton<sf::Text>;

// TextBox
void TextBox::process(Main &main){
    if(update){
        update(main,*this);
    }
    // Caret
    if(activated){
        // Set
        caret.setPosition(drawable.getPosition().x+drawable.getLocalBounds().width+5,drawable.getPosition().y);
        caret.setColor(normalColor);

    }
    //
    if(check(main.renderer.window)){
        action(main,*this);
    }
}
void TextBox::draw(sf::RenderWindow &window){
    window.draw(drawable);
    if(activated && fmod(caretClock.getElapsedTime().asSeconds(),1.0) > 0.5 ){
        window.draw(caret);
    }
}
TextBox::TextBox(sf::Text &&i_drawable, std::function<bool(Main&,ObjectButton<sf::Text>&)> &&i_visible, std::function<void(Main&,ObjectButton<sf::Text>&)> &&i_action, std::function<void(Main&,ObjectButton<sf::Text>&)> &&i_update, unsigned int &i_updateInfo, const sf::Color &i_selectedColor,const unsigned int &i_selectedStyle):
    ObjectButton(std::move(i_drawable), std::move(i_visible), std::move(i_action),std::move(i_update),i_updateInfo,i_selectedColor,i_selectedStyle){
    // Initialize caret
    caret.setString("|");
    caret.setFont(*drawable.getFont());
    caret.setCharacterSize(drawable.getCharacterSize());
}

// Line
bool Line::checkVisible(Main &main){
    if(main.player.size()>id){
        return true;
    }
    return false;
}
void Line::draw(sf::RenderWindow &window){
    window.draw(*line);
    window.draw(*noTurtleLine);
}
Line::Line(sf::VertexArray *i_line, sf::VertexArray *i_noTurtleLine, unsigned int &i_id){
    line=i_line;
    noTurtleLine=i_noTurtleLine;
    id=i_id;
}
// Head
bool Head::checkVisible(Main &main){
    if(main.player.size()>id){
        return true;
    }
    return false;
}
void Head::draw(sf::RenderWindow &window){
    //if(!*death){ // Death and darkness?
        if(*rightAngle){
            window.draw(*rectangle);
        }
        else{
            window.draw(*circle);
        }
    //}
}
Head::Head(sf::CircleShape *i_circle, sf::RectangleShape *i_rectangle, bool *i_death, bool *i_rightAngle, unsigned int &i_id){
    circle=i_circle;
    rectangle=i_rectangle;
    id=i_id;
    death=i_death;
    rightAngle=i_rightAngle;
}
// PowerupObjects
bool PowerupObjects::checkVisible(Main &main){
    return true;
}
void PowerupObjects::draw(sf::RenderWindow &window){
    for(auto powerup2 = powerups->begin(); powerup2!=powerups->end(); powerup2++){
        // Copy; Should try to embed this in the line above
        PowerupField powerup=*powerup2;
        // If darkness powerup is enabled only draw powerups within FoV
        /*if(game.darkness){
            bool draw=false;
            int diffx;
            int diffy;
            for(unsigned int j=0;j<player.size();j++){
                diffx=(player[j].x-powerup.x);
                diffy=(player[j].y-powerup.y);
                if( diffx*diffx+diffy*diffy < (config.darknessRadius*config.darknessRadius)/4 ){
                    draw=true;
                    break;
                }
            }
            if(!draw){
                continue;
            }
        }*/
        // Set opacity for fade in and out effects

        // Config not available atm; Pointer?
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
            std::cout << "Error! No sprite!" << int2str( powerup.place ) << std::endl;
            draw=&sprite.questionMarkBlue;
        }
        // Set sprite and draw
        draw->setPosition( powerup.x-config->powerupRadius , powerup.y-config->powerupRadius );
        draw->setColor(color);
        window.draw(*draw);
    }
}
PowerupObjects::PowerupObjects(const Config &i_config, std::vector<PowerupField> *i_powerups) : sprite(i_config), config(&i_config){
    powerups=i_powerups;
}
//
