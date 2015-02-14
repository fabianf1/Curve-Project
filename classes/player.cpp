// Contains functions and constructors for the config class
// Needed Header
#include "Player.h"

// Constructor
Player::Player(){
    line.setPrimitiveType(sf::Quads);
    keyL=keyR=sf::Keyboard::Unknown;
    local=server=false;
}
//
Player::Player(const sf::String &Name,const sf::Color &Color){
    name=Name;
    color=Color;
    circle.setFillColor(color);
    line.setPrimitiveType(sf::Quads);
    keyL=keyR=sf::Keyboard::Unknown;
    local=server=false;
}
// Functions
void Player::New_Game(){
    // Set points
    points=0;
    ready=false;
}
//
void Player::newRound(const Config &config,const Game &game){
    calculatePowerupEffect(config,game);
    if(!game.client[1]){
        // Set position
        x=config.wallWidth+lineWidth+config.safeSpawn + rand() % (config.windowWidth-config.statusWidth-2*config.wallWidth-lineWidth-config.safeSpawn*2+1);
        y=config.wallWidth+lineWidth+config.safeSpawn + rand() % (config.windowHeight-2*config.wallWidth-lineWidth-config.safeSpawn*2+1);
        // Set Heading
        // Left Top
        if(x<config.wallWidth+config.safeHeading&&y<config.wallWidth+config.safeHeading){
            heading=rand() % (90+1);
        }
        // Right top
        else if(x>config.windowWidth-config.statusWidth-config.wallWidth-config.safeHeading&&y<config.wallWidth+config.safeHeading){
            heading=rand() % (90+1)+90;
        }
        // Right Bottom
        else if(x>config.windowWidth-config.statusWidth-config.wallWidth-config.safeHeading&&y>config.windowHeight-config.wallWidth-config.safeHeading){
            heading=rand() % (90+1)+180;
        }
        // Left Bottom
        else if(x<config.wallWidth+config.safeHeading&&y>config.windowHeight-config.wallWidth-config.safeHeading){
            heading=rand() % (90+1)+270;
        }
        // Left
        else if(x<config.wallWidth+config.safeHeading){
            heading=rand() % (180+1)-90;
        }
        // Top
        else if(y<config.wallWidth+config.safeHeading){
            heading=rand() % (180+1);
        }
        // Right
        else if(x>config.windowWidth-config.statusWidth-config.wallWidth-config.safeHeading){
            heading=rand() % (180+1)+90;
        }
        // Bottom
        else if(y>config.windowHeight-config.wallWidth-config.safeHeading){
            heading=rand() % (180+1)+180;
        }
        // Anywhere else
        else{
            heading=rand() % (360+1);
        }
        calculateGap(config);
        //
        ready=false;
    }
    else{
        ready=true;
    }
    // Reset other vars
    left=right=false;
    line.clear();
    death=false;
    circle.setPosition(x-lineWidth/2,y-lineWidth/2);
    circle.setRadius(lineWidth/2);
    // Client
    noLine=false;
}
//
void Player::draw(sf::RenderWindow &window){
    window.draw(line);
    window.draw(circle);
}
// Server and local version of updatePosition
void Player::updatePosition(const Config &config, Game &game){
    // Sine things
    if(sine){
        sinePhase+=(game.elapsed*config.sineFrequency*2)*PI;
        lineWidth=sineLineWidth*(1+sin(sinePhase)*config.sineAmplitude);
        shift=sineShift*(1+sin(sinePhase+PI)*config.sineAmplitude);
        circle.setRadius(lineWidth/2);
    }
    // check keys
    if(local){
        left=sf::Keyboard::isKeyPressed(keyL);
        right=sf::Keyboard::isKeyPressed(keyR);
    }
    // check keyrelease
    if(keyrelease==false&&!left&&!right){
        keyrelease=true;
    }
    // Heading Change
    hOLD=heading;
    if(!rightAngle){
        // Move left
        if( (left&&!inverted) || (right&&inverted) ){
            heading-=(turn*game.elapsed);
        }
        // Move right
        if( (right&&!inverted) || (left&&inverted) ){
            heading+=(turn*game.elapsed);
        }
    }
    else{
        if( ( (left&&!inverted) || (right&&inverted) ) && keyrelease){
            heading-=90;
            keyrelease=false;
        }
        // Move right
        if( ( (right&&!inverted) || (left&&inverted) ) && keyrelease){
            heading+=90;
            keyrelease=false;
        }
    }
    //
    // Save new X and Y, but keep old ones for a while
    xOLD=x;
    yOLD=y;
    x+=cos(heading*PI/180.0)*(shift*game.elapsed);
    y+=sin(heading*PI/180.0)*(shift*game.elapsed);
    // Update Line
    // Normal
    if(!invisible){
        // Gapping
        gap[0]-=game.elapsed;
        if(gap[0]<0.0){
            if(gap[1]<0.0){
                calculateGap(config);
                addLine(xOLD,x,yOLD,y,heading,hOLD,lineWidth);
            }
            else{
                gap[1]-=game.elapsed;
            }
        }
        // Non gapping normal line add
        else{
            addLine(xOLD,x,yOLD,y,heading,hOLD,lineWidth);
        }
    }
    // Do some extra things if outside playfield, which means walls away is active
    // Left
    if(x<config.wallWidth/2){
        xOLD+=config.windowWidth-config.statusWidth-config.wallWidth;
        x+=config.windowWidth-config.statusWidth-config.wallWidth;
        // draw extra thing if not invisible
        if(!invisible&&gap[0]>0.0){
            // Set quad points
            addLine(xOLD,x,yOLD,y,heading,hOLD,lineWidth);
        }
    }
    // Top
    else if(y<config.wallWidth/2){
        yOLD+=config.windowHeight-config.wallWidth;
        y+=config.windowHeight-config.wallWidth;
        // draw extra thing if not invisible
        if(!invisible&&gap[0]>0.0){
            // Set quad points
            addLine(xOLD,x,yOLD,y,heading,hOLD,lineWidth);
        }
    }
    // Right
    else if(x>config.windowWidth-config.statusWidth-config.wallWidth/2){
        xOLD-=config.windowWidth-config.statusWidth-config.wallWidth;
        x-=config.windowWidth-config.statusWidth-config.wallWidth;
        // draw extra thing if not invisible
        if(!invisible&&gap[0]>0.0){
            // Set quad points
            addLine(xOLD,x,yOLD,y,heading,hOLD,lineWidth);
        }
    }
    // Bottom
    else if(y>config.windowHeight-config.wallWidth/2){
        yOLD-=config.windowHeight-config.wallWidth;
        y-=config.windowHeight-config.wallWidth;
        // draw extra thing if not invisible
        if(!invisible&&gap[0]>0.0){
            // Set quad points
            addLine(xOLD,x,yOLD,y,heading,hOLD,lineWidth);
        }
    }
    // Update Circle
    if(!rightAngle){
        circle.setPosition(x-lineWidth/2,y-lineWidth/2);
    }
    else{
        // check if we need to make the right angle really right
        if(!invisible&&gap[0]>0.0&&abs(heading-hOLD)>60){
            addLine(xOLD-lineWidth/2*cos(heading*PI/180.0),xOLD+lineWidth/2*cos(heading*PI/180.0),yOLD-lineWidth/2*sin(heading*PI/180.0),yOLD+lineWidth/2*sin(heading*PI/180.0),heading,heading,lineWidth);
        }
        // Rectangle update
        rectangle.setPosition(x,y);
        rectangle.setRotation(heading);
    }
}
// Client Version of updatePosition
void Player::updatePosition(const Config &config, sf::Packet &packet){
    // Unpack
    xOLD=x;
    yOLD=y;
    hOLD=heading;
    packet >> x >> y >> heading >> lineWidth >> noLine;
    // Set Circle size
    circle.setRadius(lineWidth/2);
    //
    int maxdiffw=config.windowWidth-config.statusWidth-4*config.wallWidth;
    int maxdiffh=config.windowHeight-4*config.wallWidth;
    if(!noLine&& abs(x-xOLD)<maxdiffw && abs(y-yOLD)<maxdiffh){
        addLine(xOLD,x,yOLD,y,heading,hOLD,lineWidth);
    }
    // Update Circle
    if(!rightAngle){
        circle.setPosition(x-lineWidth/2,y-lineWidth/2);
    }
    else{
        // check if we need to make the right angle really right
        if(!noLine&&abs(heading-hOLD)>60){
            addLine(xOLD-lineWidth/2*cos(heading*PI/180.0),xOLD+lineWidth/2*cos(heading*PI/180.0),yOLD-lineWidth/2*sin(heading*PI/180.0),yOLD+lineWidth/2*sin(heading*PI/180.0),heading,heading,lineWidth);
        }
        // Rectangle update
        rectangle.setPosition(x,y);
        rectangle.setRotation(heading);
    }
}
//
void Player::addLine(const float &X1,const float &X2,const float &Y1,const float &Y2,const float &H1, const float &H2,const int &lineWidth){
    sf::Vertex quad;
    quad.color=color;
    //
    quad.position = sf::Vector2f( X1+sin(H2*PI/180.0)*lineWidth/2.0, Y1-cos(H2*PI/180.0)*lineWidth/2.0);
    line.append(quad);
    //
    quad.position = sf::Vector2f( X1-sin(H2*PI/180.0)*lineWidth/2.0, Y1+cos(H2*PI/180.0)*lineWidth/2.0);
    line.append(quad);
    //
    quad.position = sf::Vector2f( X2-sin(H1*PI/180.0)*lineWidth/2.0, Y2+cos(H1*PI/180.0)*lineWidth/2.0);
    line.append(quad);
    //
    quad.position = sf::Vector2f( X2+sin(H1*PI/180.0)*lineWidth/2.0, Y2-cos(H1*PI/180.0)*lineWidth/2.0);
    line.append(quad);
}
//
void Player::calculateGap(const Config &config){
    if(!gapping){
        gap[0]=config.minToGap + ( rand() % (config.randToGap+1) ) / 1000.0;
    }
    else{
        gap[0]=config.minToGapPowerup + ( rand() % (config.rand_to_gap_powerup+1) ) / 1000.0;
    }
    gap[1]=config.minWidthGap + ( ( rand() % (config.randWidthGap+1) ) / 1000.0 );
}
//
void Player::calculatePowerupEffect(const Config &config,const Game &game){
    // Set the normal things
    shift=config.shift;
    turn=config.turn;
    lineWidth=config.lineWidth;
    rightAngle=false;
    invisible=false;
    inverted=false;
    gapping=false;
    sine=false;
    int speed=0;
    int line_size=0;
    // check for effects
    for(unsigned int i=0;i<game.playerPowerupEffect.size();i++){
        if(game.playerPowerupEffect[i].impact==Powerup::Impact::All
           ||(game.playerPowerupEffect[i].impact==Powerup::Impact::Other&&game.playerPowerupEffect[i].player!=place)
           ||(game.playerPowerupEffect[i].impact==Powerup::Impact::Self&&game.playerPowerupEffect[i].player==place)){
            // Speed
            if(game.playerPowerupEffect[i].type==Powerup::Type::Fast){
                speed++;
            }
            else if(game.playerPowerupEffect[i].type==Powerup::Type::Slow){
                speed--;
            }
            // Linewidth
            else if(game.playerPowerupEffect[i].type==Powerup::Type::Small){
                line_size--;
            }
            else if(game.playerPowerupEffect[i].type==Powerup::Type::Big){
                line_size++;
            }
            // Right Angle
            else if(game.playerPowerupEffect[i].type==Powerup::Type::RightAngle){
                rightAngle=true;
            }
            // Invisible
            else if(game.playerPowerupEffect[i].type==Powerup::Type::Invisible){
                invisible=true;
            }
            // Invert directions
            else if(game.playerPowerupEffect[i].type==Powerup::Type::InvertKeys){
                inverted=true;
            }
            // Gapping
            else if(game.playerPowerupEffect[i].type==Powerup::Type::Gap){
                gapping=true;
                gap[0]=0;
            }
            // Sine
            else if(game.playerPowerupEffect[i].type==Powerup::Type::Sine){
                sine=true;
            }
        }
    }
    // Do speed and line size effects
    //
    if(speed>0){
        // To make sure it won't go to fast I could use a function like max*(1-exp(-a*x)). Max is the maximum speed multiplier and a is a scaling factor
        shift*=config.fastMaxMultiplier * ( 1 - exp( - config.fastScaling * speed ) ); // 2.1742    3.5606    4.4446 (max=6&a=0.45)
        turn*=config.fastTurnMaxMultiplier * ( 1 - exp( - config.fastTurnScaling * speed ) );
    }
    else if(speed<0){
        // Function min+exp(a*x); min is the minimum multiplier and a is a scaling factor. As x is already negative no minus sign is needed.
        //float multiplier=config.min_slow_multiplier+exp(config.slowScaling*speed); // -1=0.6;-2=0.36;-3=0.22 (min=0&a=0.5)
        shift*=config.slowMinMultiplier+exp(config.slowScaling*speed);;
        turn*=config.slowTurnMinMultiplier+exp(config.slowTurnScaling*speed);;
    }
    // Linewidth is calculated on the server, Same goes for the sine things
    if(!game.client[1]){
        if(line_size>0){
            lineWidth*=pow(config.bigMultiplier,line_size);
        }
        else if(line_size<0){
            lineWidth*=pow(config.smalMultiplier,(-1)*line_size);
            if(lineWidth<1){
                lineWidth=1;
            }
        }
        // Sine things
        if(!sine){
            sinePhase=0;
        }
        else{
            sineLineWidth=lineWidth;
            sineShift=shift;
        }
    }
    // Right Angle
    if(!rightAngle){
        // Circle Style
        if(inverted){
            circle.setFillColor(sf::Color::Blue);
        }
        else{
            circle.setFillColor(color);
        }
        circle.setRadius(lineWidth/2);
        // Rectangle style
    }
    else{
        rectangle.setSize(sf::Vector2f(lineWidth,lineWidth));
        if(inverted){
            rectangle.setFillColor(sf::Color::Blue);
        }
        else{
            rectangle.setFillColor(color);
        }
        rectangle.setOrigin(lineWidth/2,lineWidth/2);
    }
}
//
