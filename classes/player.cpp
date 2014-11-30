// Contains functions and constructors for the config class
// Needed Header
#include "..\curve.h"
#include <math.h>
// Constructor
Player::Player(){
    line.setPrimitiveType(sf::Quads);
}
// Functions
void Player::New_Round(const Config &config,const Game &game){
    //
    x=config.wallwidth+linewidth+config.safespawn + rand() % (config.window_width-config.statuswidth-2*config.wallwidth-linewidth-config.safespawn*2+1); // Rand x position within walls
    y=config.wallwidth+linewidth+config.safespawn + rand() % (config.window_height-2*config.wallwidth-linewidth-config.safespawn*2+1);
    // Safe Heading :D
    // Left Top
    if(x<config.wallwidth+config.safeheading&&y<config.wallwidth+config.safeheading){
        h=rand() % (90+1);
    }
    // Right top
    else if(x>config.window_width-config.statuswidth-config.wallwidth-config.safeheading&&y<config.wallwidth+config.safeheading){
        h=rand() % (90+1)+90;
    }
    // Right Bottom
    else if(x>config.window_width-config.statuswidth-config.wallwidth-config.safeheading&&y>config.window_height-config.wallwidth-config.safeheading){
        h=rand() % (90+1)+180;
    }
    // Left Bottom
    else if(x<config.wallwidth+config.safeheading&&y>config.window_height-config.wallwidth-config.safeheading){
        h=rand() % (90+1)+270;
    }
    // Left
    else if(x<config.wallwidth+config.safeheading){
        h=rand() % (180+1)-90;
    }
    // Top
    else if(y<config.wallwidth+config.safeheading){
        h=rand() % (180+1);
    }
    // Right
    else if(x>config.window_width-config.statuswidth-config.wallwidth-config.safeheading){
        h=rand() % (180+1)+90;
    }
    // Bottom
    else if(y>config.window_height-config.wallwidth-config.safeheading){
        h=rand() % (180+1)+180;
    }
    // Anywhere else
    else{
        h=rand() % (360+1);
    }
    //
    line.clear();
    death=false;
    deathframe=0;
    Calculate_Powerup_Effect(config,game);
    Calculate_Gap(config);
    circle.setPosition(x-linewidth/2,y-linewidth/2);
    circle.setRadius(linewidth/2);
    //keyrelease=false;
}
//
void Player::New_Round_Server(const Config &config,Game &game,int i){
    New_Round(config,game);
    Pending pending;
    pending.packet << Packet::NewRound << i << x << y << h;
    pending.send_id.push_back(-1);
    game.mutex.lock();
    game.pending.push_back(pending);
    game.mutex.unlock();
    noline=false;
    sendlast=false;
    //wall=false;
}
//
void Player::New_Round_Client(const Config &config,Game &game,sf::Packet &packet){
    // Unpack
    packet >> x >> y >> h;
    //
    ready=true;
    line.clear();
    Calculate_Powerup_Effect(config,game);
    circle.setPosition(x-linewidth/2,y-linewidth/2);
    circle.setRadius(linewidth/2); // Error HEre!!!!!!
}
//
void Player::Update_Position(const Config &config, Game &game){
    // Check keyrelease
    if(keyrelease==false&&!left&&!right){
        keyrelease=true;
    }
    // Heading Change
    hOLD=h;
    if(!rightangle){
        // Move left
        if( (left&&!inverted) || (right&&inverted) ){
            h-=(turn*game.elapsed);
        }
        // Move right
        if( (right&&!inverted) || (left&&inverted) ){
            h+=(turn*game.elapsed);
        }
    }
    else{
        if( ( (left&&!inverted) || (right&&inverted) ) && keyrelease){
            h-=90;
            keyrelease=false;
        }
        // Move right
        if( ( (right&&!inverted) || (left&&inverted) ) && keyrelease){
            h+=90;
            keyrelease=false;
        }
    }
    // Force between 0-360 degrees
    if(h>360){
        h-=360;
    }
    else if(h<0){
        h+=360;
    }
    //
    // Save new X and Y, but keep old ones for a while
    xOLD=x;
    yOLD=y;
    x+=cos(h*PI/180.0)*(shift*game.elapsed);
    y+=sin(h*PI/180.0)*(shift*game.elapsed);
    // Update Line
    // Normal
    if(!invisible){
        // Gapping
        gap[0]-=game.elapsed;
        if(gap[0]<0.0){
            if(gap[1]<0.0){
                Calculate_Gap(config);
                Add_Line(xOLD,x,yOLD,y,h,hOLD,linewidth);
            }
            else{
                gap[1]-=game.elapsed;
            }
        }
        // Non gapping normal line add
        else{
            Add_Line(xOLD,x,yOLD,y,h,hOLD,linewidth);
        }
    }
    // Do some extra things if outside playfield, which means if hitdetector works walls away is active
    // Left
    if(x<config.wallwidth/2){
        xOLD+=config.window_width-config.statuswidth-config.wallwidth;
        x+=config.window_width-config.statuswidth-config.wallwidth;
        // Draw extra thing if not invisible
        if(!invisible){
            // Set quad points
            Add_Line(xOLD,x,yOLD,y,h,hOLD,linewidth);
        }
    }
    // Top
    else if(y<config.wallwidth/2){
        yOLD+=config.window_height-config.wallwidth;
        y+=config.window_height-config.wallwidth;
        // Draw extra thing if not invisible
        if(!invisible){
            // Set quad points
            Add_Line(xOLD,x,yOLD,y,h,hOLD,linewidth);
        }
    }
    // Right
    else if(x>config.window_width-config.statuswidth-config.wallwidth/2){
        xOLD-=config.window_width-config.statuswidth-config.wallwidth;
        x-=config.window_width-config.statuswidth-config.wallwidth;
        // Draw extra thing if not invisible
        if(!invisible){
            // Set quad points
            Add_Line(xOLD,x,yOLD,y,h,hOLD,linewidth);
        }
    }
    // Bottom
    else if(y>config.window_height-config.wallwidth/2){
        yOLD-=config.window_height-config.wallwidth;
        y-=config.window_height-config.wallwidth;
        // Draw extra thing if not invisible
        if(!invisible){
            // Set quad points
            Add_Line(xOLD,x,yOLD,y,h,hOLD,linewidth);
        }
    }
    // Update Circle
    circle.setPosition(x-linewidth/2,y-linewidth/2);
}
//
void Player::Update_Position_Local(const Config &config, Game &game){
    left=sf::Keyboard::isKeyPressed(keyL);
    right=sf::Keyboard::isKeyPressed(keyR);
    Update_Position(config,game);
}
//
/*void Player::Update_Position_Server(const Config &config, Game &game,int i){
    Update_Position(config,game);
    // Big package if last position was not sent to make sure gapwidth is the same by players
    if( ( (!noline && (invisible||gap[0]<0.0) ) || (noline&& !invisible && gap[0]>0.0) ) && send ){
        noline=!noline;
        // Send Big Package
        packetnumber++;
        Pending pending;
        pending.send_id.push_back(-1);
        pending.packet << Packet::Update2 << i << xOLD << yOLD << hOLD << !noline << packetnumber << x << y << h << noline;
        game.mutex.lock();
        game.pending.push_back(pending);
        game.mutex.unlock();
        send=false;
    }
    else{
        if( (!noline&& (invisible||gap[0]<0.0) ) || (noline&&!invisible&&gap[0]>0.0) ){
            noline=!noline;
            send=true;
        }
        // Send Normal Package
        if(send){
            packetnumber++;
            Pending pending;
            pending.send_id.push_back(-1);
            pending.packet << Packet::Update << i << x << y << h << noline << packetnumber;
            game.mutex.lock();
            game.pending.push_back(pending);
            game.mutex.unlock();
            send=false;
        }
        else{
            send=true;
        }
    }
}*/
//
void Player::Update_Position_Client(const Config &config, sf::Packet &packet){
    xOLD=x;
    yOLD=y;
    hOLD=h;
    packet >> x >> y >> h >> noline;
    //
    int maxdiffw=config.window_width-config.statuswidth-4*config.wallwidth;
    int maxdiffh=config.window_height-4*config.wallwidth;
    if(!noline&& abs(x-xOLD)<maxdiffw && abs(y-yOLD)<maxdiffh){
        Add_Line(xOLD,x,yOLD,y,h,hOLD,linewidth);
    }
    // Circle
    circle.setPosition(x-linewidth/2,y-linewidth/2);
}
//
/*void Player::Update_Position_Client2(const Config &config, sf::Packet &packet){
    xOLD=x;
    yOLD=y;
    hOLD=h;
    int number;
    packet >> x >> y >> h >> noline >> number;
    if(number>packetnumber){
        // Phase 1
        int maxdiffw=config.window_width-config.statuswidth-4*config.wallwidth;
        int maxdiffh=config.window_height-4*config.wallwidth;
        if(!noline&& abs(x-xOLD)<maxdiffw && abs(y-yOLD)<maxdiffh){
            Add_Line(xOLD,x,yOLD,y,h,hOLD,linewidth);
        }
        // Phase 2
        xOLD=x;
        yOLD=y;
        hOLD=h;
        packet >> x >> y >> h >> noline;
        if(!noline&& abs(x-xOLD)<maxdiffw && abs(y-yOLD)<maxdiffh){
            Add_Line(xOLD,x,yOLD,y,h,hOLD,linewidth);
        }
    }
    else{
        std::cout << "Very Bad Packet mixup!! " << number << "," << packetnumber << " , " << x << ", " << y << ", " << h << std::endl;
    }
}*/
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
void Player::Calculate_Powerup_Effect(const Config &config,const Game &game){
    // Set the normal things
    shift=config.shift;
    turn=config.turn;
    linewidth=config.linewidth;
    rightangle=false;
    invisible=false;
    inverted=false;
    int speed=0;
    int line_size=0;
    // Check for effects
    for(unsigned int i=0;i<game.player_powerup_effect.size();i++){
        if(game.player_powerup_effect[i].impact==2||(game.player_powerup_effect[i].impact==1&&game.player_powerup_effect[i].player!=place)||(game.player_powerup_effect[i].impact==0&&game.player_powerup_effect[i].player==place)){
            // Speed
            if(game.player_powerup_effect[i].type==0){
                speed++;
            }
            else if(game.player_powerup_effect[i].type==1){
                speed--;
            }
            // Linewidth
            else if(game.player_powerup_effect[i].type==2){
                line_size--;
            }
            else if(game.player_powerup_effect[i].type==3){
                line_size++;
            }
            // Right Angle
            else if(game.player_powerup_effect[i].type==4){
                rightangle=true;
            }
            // Invisible
            else if(game.player_powerup_effect[i].type==6){
                invisible=true;
            }
            // Invert directions
            else if(game.player_powerup_effect[i].type==9){
                inverted=true;
            }
        }
    }
    // Do speed and line size effects
    //
    if(speed>0){
        // Making sure speed doesn't get too high
        if(speed>3){
            speed=3;
        }
        shift*=pow(config.fast_multiplier,speed);
        turn*=pow(config.fast_multiplier,speed);
    }
    else if(speed<0){
        // Making sure speed doesn't get too low
        if(speed<-3){
            speed=-3;
        }
        shift*=pow(config.slow_multiplier,(-1)*speed);
        turn*=pow(config.slow_multiplier,(-1)*speed);
    }
    //
    if(line_size>0){
        // No limit on linewidth :D
        linewidth*=pow(config.big_multiplier,line_size);
    }
    else if(line_size<0){
        linewidth*=pow(config.small_multiplier,(-1)*line_size);
        if(linewidth==0){
            linewidth=1;
        }
    }
    // Circle Style
    if(inverted){
        circle.setFillColor(sf::Color::Blue);
    }
    else{
        circle.setFillColor(color);
    }
    circle.setRadius(linewidth/2);
}
//
void Player::Calculate_Gap(const Config &config){
    gap[0]=config.min_to_gap + ( rand() % (config.rand_to_gap+1) ) / 1000.0;
    gap[1]=config.min_width_gap + ( ( rand() % (config.rand_width_gap+1) ) / 1000.0 );
}
//
void Player::Process_SYNC_Packet(sf::Packet &packet){
    packet >> name >> enabled >> ready;
}
//
void Player::Name_Packet(Game &game){
    //
    if(name.isEmpty()){
        name="Player";
    }
    //
    Pending pending;
    pending.packet << Packet::Name << game.id << name;
    pending.send_id.push_back(-1);
    game.mutex.lock();
    game.pending.push_back(pending);
    game.mutex.unlock();
}
