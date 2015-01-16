// Contains functions and constructors for the config class
// Needed Header
#include "player.h"

// Constructor
Player::Player(){
    line.setPrimitiveType(sf::Quads);
    keyL=keyR=sf::Keyboard::Unknown;
    local=server=false;
    disconnected=false;
}
//
Player::Player(const sf::String &Name,const sf::Color &Color){
    name=Name;
    color=Color;
    circle.setFillColor(color);
    line.setPrimitiveType(sf::Quads);
    keyL=keyR=sf::Keyboard::Unknown;
    local=server=false;
    disconnected=false;
}
// Functions
void Player::New_Game(){
    // Set points
    points=0;
    ready=false;
}
//
void Player::New_Round(const Config &config,const Game &game){
    Calculate_Powerup_Effect(config,game);
    if(!game.client[1]){
        // Set position
        x=config.wallwidth+linewidth+config.safespawn + rand() % (config.window_width-config.statuswidth-2*config.wallwidth-linewidth-config.safespawn*2+1);
        y=config.wallwidth+linewidth+config.safespawn + rand() % (config.window_height-2*config.wallwidth-linewidth-config.safespawn*2+1);
        // Set Heading
        // Left Top
        if(x<config.wallwidth+config.safeheading&&y<config.wallwidth+config.safeheading){
            heading=rand() % (90+1);
        }
        // Right top
        else if(x>config.window_width-config.statuswidth-config.wallwidth-config.safeheading&&y<config.wallwidth+config.safeheading){
            heading=rand() % (90+1)+90;
        }
        // Right Bottom
        else if(x>config.window_width-config.statuswidth-config.wallwidth-config.safeheading&&y>config.window_height-config.wallwidth-config.safeheading){
            heading=rand() % (90+1)+180;
        }
        // Left Bottom
        else if(x<config.wallwidth+config.safeheading&&y>config.window_height-config.wallwidth-config.safeheading){
            heading=rand() % (90+1)+270;
        }
        // Left
        else if(x<config.wallwidth+config.safeheading){
            heading=rand() % (180+1)-90;
        }
        // Top
        else if(y<config.wallwidth+config.safeheading){
            heading=rand() % (180+1);
        }
        // Right
        else if(x>config.window_width-config.statuswidth-config.wallwidth-config.safeheading){
            heading=rand() % (180+1)+90;
        }
        // Bottom
        else if(y>config.window_height-config.wallwidth-config.safeheading){
            heading=rand() % (180+1)+180;
        }
        // Anywhere else
        else{
            heading=rand() % (360+1);
        }
        Calculate_Gap(config);
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
    deathframe=0;
    circle.setPosition(x-linewidth/2,y-linewidth/2);
    circle.setRadius(linewidth/2);
    // Lower two are for server things?
    noline=false;
    sendlast=false;
    // Client part

}
//
void Player::Draw(sf::RenderWindow &window){
    window.draw(line);
    window.draw(circle);
}
//
void Player::Update_Position(const Config &config, Game &game){
    // Sine things
    if(sine){
        sine_phase+=(game.elapsed*config.sine_frequency*2)*PI;
        linewidth=sine_linewidth*(1+sin(sine_phase)*config.sine_amplitude);
        shift=sine_shift*(1+sin(sine_phase+PI)*config.sine_amplitude);
        circle.setRadius(linewidth/2);
    }
    // Check keys
    if(local){
        left=sf::Keyboard::isKeyPressed(keyL);
        right=sf::Keyboard::isKeyPressed(keyR);
    }
    // Check keyrelease
    if(keyrelease==false&&!left&&!right){
        keyrelease=true;
    }
    // Heading Change
    hOLD=heading;
    if(!rightangle){
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
                Calculate_Gap(config);
                Add_Line(xOLD,x,yOLD,y,heading,hOLD,linewidth);
            }
            else{
                gap[1]-=game.elapsed;
            }
        }
        // Non gapping normal line add
        else{
            Add_Line(xOLD,x,yOLD,y,heading,hOLD,linewidth);
        }
    }
    // Do some extra things if outside playfield, which means walls away is active
    // Left
    if(x<config.wallwidth/2){
        xOLD+=config.window_width-config.statuswidth-config.wallwidth;
        x+=config.window_width-config.statuswidth-config.wallwidth;
        // Draw extra thing if not invisible
        if(!invisible){
            // Set quad points
            Add_Line(xOLD,x,yOLD,y,heading,hOLD,linewidth);
        }
    }
    // Top
    else if(y<config.wallwidth/2){
        yOLD+=config.window_height-config.wallwidth;
        y+=config.window_height-config.wallwidth;
        // Draw extra thing if not invisible
        if(!invisible){
            // Set quad points
            Add_Line(xOLD,x,yOLD,y,heading,hOLD,linewidth);
        }
    }
    // Right
    else if(x>config.window_width-config.statuswidth-config.wallwidth/2){
        xOLD-=config.window_width-config.statuswidth-config.wallwidth;
        x-=config.window_width-config.statuswidth-config.wallwidth;
        // Draw extra thing if not invisible
        if(!invisible){
            // Set quad points
            Add_Line(xOLD,x,yOLD,y,heading,hOLD,linewidth);
        }
    }
    // Bottom
    else if(y>config.window_height-config.wallwidth/2){
        yOLD-=config.window_height-config.wallwidth;
        y-=config.window_height-config.wallwidth;
        // Draw extra thing if not invisible
        if(!invisible){
            // Set quad points
            Add_Line(xOLD,x,yOLD,y,heading,hOLD,linewidth);
        }
    }
    // Update Circle
    if(!rightangle){
        circle.setPosition(x-linewidth/2,y-linewidth/2);
    }
    else{
        rectangle.setPosition(x,y);
        rectangle.setRotation(heading);
    }
}
//
void Player::Update_Position(const Config &config, sf::Packet &packet,const float &packettime){
    // Sine things
    if(sine){
        sine_phase+=(packettime*config.sine_frequency*2)*PI;
        linewidth=sine_linewidth*(1+sin(sine_phase)*config.sine_amplitude);
        shift=sine_shift*(1+sin(sine_phase+PI)*config.sine_amplitude);
        circle.setRadius(linewidth/2);
    }
    xOLD=x;
    yOLD=y;
    hOLD=heading;
    packet >> x >> y >> heading >> noline;
    //
    int maxdiffw=config.window_width-config.statuswidth-4*config.wallwidth;
    int maxdiffh=config.window_height-4*config.wallwidth;
    if(!noline&& abs(x-xOLD)<maxdiffw && abs(y-yOLD)<maxdiffh){
        Add_Line(xOLD,x,yOLD,y,heading,hOLD,linewidth);
    }
    // Circle
    circle.setPosition(x-linewidth/2,y-linewidth/2);
}
//
void Player::Add_Line(const float &X1,const float &X2,const float &Y1,const float &Y2,const float &H1, const float &H2,const int &linewidth){
    sf::Vertex quad;
    quad.color=color;
    //
    quad.position = sf::Vector2f( X1+sin(H2*PI/180.0)*linewidth/2.0, Y1-cos(H2*PI/180.0)*linewidth/2.0);
    line.append(quad);
    //
    quad.position = sf::Vector2f( X1-sin(H2*PI/180.0)*linewidth/2.0, Y1+cos(H2*PI/180.0)*linewidth/2.0);
    line.append(quad);
    //
    quad.position = sf::Vector2f( X2-sin(H1*PI/180.0)*linewidth/2.0, Y2+cos(H1*PI/180.0)*linewidth/2.0);
    line.append(quad);
    //
    quad.position = sf::Vector2f( X2+sin(H1*PI/180.0)*linewidth/2.0, Y2-cos(H1*PI/180.0)*linewidth/2.0);
    line.append(quad);
}
//
void Player::Calculate_Gap(const Config &config){
    if(!gapping){
        gap[0]=config.min_to_gap + ( rand() % (config.rand_to_gap+1) ) / 1000.0;
    }
    else{
        gap[0]=config.min_to_gap_powerup + ( rand() % (config.rand_to_gap_powerup+1) ) / 1000.0;
    }
    gap[1]=config.min_width_gap + ( ( rand() % (config.rand_width_gap+1) ) / 1000.0 );
}
//
void Player::Calculate_Powerup_Effect(const Config &config,const Game &game){
    // Set the normal things
    shift=config.shift;
    turn=config.turn;
    linewidth=config.linewidth;
    rightangle=false;
    invisible=false;
    inverted=false;
    gapping=false;
    sine=false;
    int speed=0;
    int line_size=0;
    // Check for effects
    for(unsigned int i=0;i<game.player_powerup_effect.size();i++){
        if(game.player_powerup_effect[i].impact==Powerup::Impact::All
           ||(game.player_powerup_effect[i].impact==Powerup::Impact::Other&&game.player_powerup_effect[i].player!=place)
           ||(game.player_powerup_effect[i].impact==Powerup::Impact::Self&&game.player_powerup_effect[i].player==place)){
            // Speed
            if(game.player_powerup_effect[i].type==Powerup::Type::Fast){
                speed++;
            }
            else if(game.player_powerup_effect[i].type==Powerup::Type::Slow){
                speed--;
            }
            // Linewidth
            else if(game.player_powerup_effect[i].type==Powerup::Type::Small){
                line_size--;
            }
            else if(game.player_powerup_effect[i].type==Powerup::Type::Big){
                line_size++;
            }
            // Right Angle
            else if(game.player_powerup_effect[i].type==Powerup::Type::Right_Angle){
                rightangle=true;
            }
            // Invisible
            else if(game.player_powerup_effect[i].type==Powerup::Type::Invisible){
                invisible=true;
            }
            // Invert directions
            else if(game.player_powerup_effect[i].type==Powerup::Type::Invert_Keys){
                inverted=true;
            }
            // Gapping
            else if(game.player_powerup_effect[i].type==Powerup::Type::Gap){
                gapping=true;
                gap[0]=0;
            }
            // Sine
            else if(game.player_powerup_effect[i].type==Powerup::Type::Sine){
                sine=true;
            }
        }
    }
    // Do speed and line size effects
    //
    if(speed>0){
        // To make sure it won't go to fast I could use a function like max*(1-exp(-a*x)). Max is the maximum speed multiplier and a is a scaling factor
        shift*=config.fast_max_multiplier * ( 1 - exp( - config.fast_scaling * speed ) ); // 2.1742    3.5606    4.4446 (max=6&a=0.45)
        turn*=config.fast_turn_max_multiplier * ( 1 - exp( - config.fast_turn_scaling * speed ) );
    }
    else if(speed<0){
        // Function min+exp(a*x); min is the minimum multiplier and a is a scaling factor. As x is already negative no minus sign is needed.
        //float multiplier=config.min_slow_multiplier+exp(config.slow_scaling*speed); // -1=0.6;-2=0.36;-3=0.22 (min=0&a=0.5)
        shift*=config.slow_min_multiplier+exp(config.slow_scaling*speed);;
        turn*=config.slow_turn_min_multiplier+exp(config.slow_turn_scaling*speed);;
    }
    //
    if(line_size>0){
        linewidth*=pow(config.big_multiplier,line_size);
    }
    else if(line_size<0){
        linewidth*=pow(config.small_multiplier,(-1)*line_size);
        if(linewidth<1){
            linewidth=1;
        }
    }
    // Sine things
    if(!sine){
        sine_phase=0;
    }
    else{
        sine_linewidth=linewidth;
        sine_shift=shift;
    }
    if(!rightangle){
        // Circle Style
        if(inverted){
            circle.setFillColor(sf::Color::Blue);
        }
        else{
            circle.setFillColor(color);
        }
        circle.setRadius(linewidth/2);
        // Rectangle style
    }
    else{
        rectangle.setSize(sf::Vector2f(linewidth,linewidth));
        if(inverted){
            rectangle.setFillColor(sf::Color::Blue);
        }
        else{
            rectangle.setFillColor(color);
        }
        rectangle.setOrigin(linewidth/2,linewidth/2);
    }
}
//
