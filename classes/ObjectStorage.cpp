// Contains functions and constructors for the ObjectStorage class
// Needed Header
#include "ObjectStorage.h"
#include "main.h"
// Constructor
ObjectStorage::ObjectStorage(const Config &config): font(loadFont("fontdata")){
    // Initialize main menu
    initMainMenu(config);
}
//
void ObjectStorage::initMainMenu(const Config &config){
    objects.clear();
    mode=Game::Mode::mainMenu;
    // Create objects to be used
    sf::Text tempText("",font);
    std::function<bool(Main&, ObjectButton<sf::Text>&)> trueFunction=[](Main &main,ObjectButton<sf::Text> &button){return true;};
    std::function<bool(Main&, ObjectButton<sf::Text>&)> visible;
    std::function<void(Main&, ObjectButton<sf::Text>&)> action;
    std::function<void(Main&, ObjectButton<sf::Text>&)> update;
    // Title
    tempText.setString("Curve Project");
    tempText.setCharacterSize(config.fontTitleSize);
    tempText.setColor(sf::Color::White);
    tempText.setStyle(sf::Text::Bold);
    tempText.setPosition( (config.windowWidth / 2) - (tempText.getLocalBounds().width / 2), 10); // Middle Top
    addToVector(tempText);
    // Version
    tempText.setString(config.majorVersion + "." + config.minorVersion);
    tempText.setCharacterSize(config.fontSize);
    //tempText.setColor(sf::Color::White);
    //tempText.setStyle(sf::Text::Bold);
    tempText.setPosition( (config.windowWidth / 2) - (tempText.getLocalBounds().width/2), 60); // Middle top
    addToVector(tempText);
    // Buttons
    // Start
    tempText.setString("Create Game");
    tempText.setCharacterSize(config.fontSize);
    tempText.setColor(sf::Color::Red);
    //tempText.setStyle(sf::Text::Bold);
    tempText.setPosition(50, config.windowHeight / 2); // Left middle
    action = [](Main &main, ObjectButton<sf::Text> &button){
        main.gameSetup.initialize(main.config,main.game,main.player);
        main.storage.initGameSetup(main.config);
    };
    addToVector(tempText,trueFunction,action,sf::Color::Yellow,sf::Text::Italic|sf::Text::Bold);
    // Connect to server
    tempText.setString("Join Game");
    action = [](Main &main, ObjectButton<sf::Text> &button){
        // Open IP Prompt
        //button.activated=!button.activated;
        main.game.joinGame=!main.game.joinGame;
    };
    update = [](Main &main, ObjectButton<sf::Text> &button){
        if(main.game.joinGame){
            button.drawable.setString("Enter IP: ");
        }
        else{
            button.drawable.setString("Join Game");
        }
    };
    tempText.setPosition( 50 , config.windowHeight/2+40); // Left ~middle
    unsigned int random = 0; // Need to fix not accepting non unsigned
    addToVector(tempText, trueFunction, action, update, random, sf::Color::Yellow, sf::Text::Italic);
    // Quit
    tempText.setString("Quit");
    action = [](Main &main, ObjectButton<sf::Text> &button){
        main.renderer.window.close();
    };
    tempText.setPosition( 50 , config.windowHeight/2+80); // Left ~middle
    addToVector(tempText,trueFunction,action,sf::Color::Yellow,sf::Text::Italic|sf::Text::Bold);
    // Prompt
    // IP textbox
    tempText.setPosition(180, config.windowHeight/2+40); // Left ~middle
    visible = [](Main &main, ObjectButton<sf::Text> &button){
        if(main.game.joinGame){
            button.activated=true;
            return true;
        }
        else{
            return false;
        }
    };
    action = [](Main &main, ObjectButton<sf::Text> &button){
        //button.activated=!button.activated;
    };
    update = [](Main &main, ObjectButton<sf::Text> &button){
        button.drawable.setString(main.game.serverIp);
    };
    addToVector2(tempText, visible, action, update, random, sf::Color::Yellow, sf::Text::Italic);
    // Join button
    tempText.setString("Join");
    tempText.setPosition(470, config.windowHeight/2+40); // Left ~middle
    action = [](Main &main, ObjectButton<sf::Text> &button){
        if(main.game.serverIp.size()>6){
            main.client.start(main.config,main.game,main.player);
            if(main.game.client[1]){
                main.storage.initGameSetup(main.config);
            }
        }
    };
    addToVector(tempText, visible, action, sf::Color::Yellow, sf::Text::Italic);
}
//
void ObjectStorage::initGameSetup(const Config &config){
    objects.clear();
    mode=Game::Mode::setup;
    // Temporary objects
    sf::Text tempText("",font);
    std::function<bool(Main&, ObjectButton<sf::Text>&)> trueFunction=[](Main &main,ObjectButton<sf::Text> &button){return true;};
    std::function<bool(Main&, ObjectButton<sf::Text>&)> visible;
    std::function<void(Main&, ObjectButton<sf::Text>&)> action;
    unsigned int random=0;
    sf::Text drawable;
    // Title
    tempText.setString("Game setup");
    tempText.setFont(font);
    tempText.setCharacterSize(config.fontTitleSize);
    tempText.setColor(sf::Color::White);
    tempText.setStyle(sf::Text::Bold);
    tempText.setPosition( (config.windowWidth/2)-(tempText.getLocalBounds().width/2) ,10); // Middle Top
    addToVector(tempText);
    // Name
    tempText.setString("Name");
    tempText.setFont(font);
    tempText.setCharacterSize(config.fontSize);
    tempText.setColor(sf::Color::White);
    tempText.setStyle(sf::Text::Bold);
    tempText.setPosition(40 , 150); // Left Semi-top
    addToVector(tempText);
    // Left
    tempText.setString("Left");
    tempText.setFont(font);
    tempText.setCharacterSize(config.fontSize);
    tempText.setColor(sf::Color::White);
    tempText.setStyle(sf::Text::Bold);
    tempText.setPosition(200 , 150);
    addToVector(tempText);
    // Right
    tempText.setString("Right");
    tempText.setFont(font);
    tempText.setCharacterSize(config.fontSize);
    tempText.setColor(sf::Color::White);
    tempText.setStyle(sf::Text::Bold);
    tempText.setPosition(350 , 150);
    addToVector(tempText);
    // Add player
    tempText.setString("Add");
    tempText.setFont(font);
    tempText.setCharacterSize(config.fontSize);
    tempText.setColor(sf::Color::Red);
    tempText.setStyle(sf::Text::Regular);
    tempText.setPosition(40 , 150+30*1.5);
    visible=[](Main &main,ObjectButton<sf::Text> &button){
        if(main.player.size()<main.config.maxPlayers && ( (main.game.client[1]&&!main.client.ready&&main.game.multiplePlayersEnabled) || !main.game.client[1] ) ){
            return true;
        }
        else{
            return false;
        }
    };
    action = [](Main &main, ObjectButton<sf::Text> &button){
        // Send request
        if(main.game.client[1]){
            Pending pending;
            pending.packet << Packet::RequestPlayer;
            main.game.queuePacket(pending);
        }
        else{
            // Add new player
            main.gameSetup.addPlayer(main.game,main.player);
            // UI is automatically Refreshed
            // Send to clients
            if(main.game.server[1]){
               main.server.syncClients(main.game,main.player);
            }
        }
    };
    std::function<void(Main&,ObjectButton<sf::Text>&)> yupdate = [](Main &main, ObjectButton<sf::Text> &button){
        // Edit position
        button.drawable.setPosition(40 , 150+45*(main.player.size()+1) );
    };
    addToVector(tempText,visible,action,yupdate,random,sf::Color::Yellow,sf::Text::Italic);
    // Start Game
    tempText.setString("Start Game");
    tempText.setFont(font);
    tempText.setCharacterSize(config.fontSize);
    tempText.setColor(sf::Color::Red);
    tempText.setStyle(sf::Text::Regular);
    tempText.setPosition(800-tempText.getLocalBounds().width/2,config.windowHeight-150);
    action = [](Main &main,ObjectButton<sf::Text> &button){
        if(!main.game.client[1]){
            if(main.gameSetup.startGame(main.config,main.game,main.player,main.server)){
                main.storage.initGame(main.config,main.game, main.player);
            };
        }
        else{
            main.game.nameChange=-1;
            main.game.keyChange[0]=-1;
            // Client Things
            main.client.toggleReady(main.game,main.player);
        }
    };
    yupdate = [](Main &main, ObjectButton<sf::Text> &button){
        if(!main.game.client[1]){
            button.drawable.setString("Start Game");
        }
        else if(!main.client.ready){
            button.drawable.setString("Ready?");
        }
        else{
            button.drawable.setString("Waiting...");
        }
        button.drawable.setPosition(800-button.drawable.getLocalBounds().width/2,main.config.windowHeight-150);
    };
    addToVector(tempText,trueFunction,action, yupdate, random, sf::Color::Yellow,sf::Text::Italic);
    // Return to main menu
    tempText.setString("Quit");
    tempText.setFont(font);
    tempText.setCharacterSize(config.fontSize);
    tempText.setColor(sf::Color::Red);
    tempText.setStyle(sf::Text::Regular);
    tempText.setPosition(800-tempText.getLocalBounds().width/2,config.windowHeight-100);
    action = [](Main &main,ObjectButton<sf::Text> &button){
        main.gameSetup.quit(main.config,main.game,main.player);
        main.storage.initMainMenu(main.config);
    };
    addToVector(tempText,trueFunction,action,sf::Color::Yellow,sf::Text::Italic);
    // Players
    for(unsigned int i=0;i<config.maxPlayers;i++){
        // Name
        tempText.setString("Player");
        tempText.setFont(font);
        tempText.setCharacterSize(config.fontSize);
        tempText.setColor(sf::Color::Red);
        tempText.setStyle(sf::Text::Regular);
        tempText.setPosition(40,195+(i)*45);
        unsigned int id=i;
        visible=[](Main &main,ObjectButton<sf::Text> &button){
            if(main.player.size()>button.id){
                return true;
            }
            return false;
        };
        action = [](Main &main, ObjectButton<sf::Text> &button){
            // Action if local and not ready
            if(main.player[button.id].local && (!main.game.client[1] || (main.game.client[1]&&!main.client.ready) ) ){
                // Disable change
                if(main.game.nameChange==button.id){
                    if(main.player[button.id].nameChange.getSize()>0){
                        main.player[button.id].name=main.player[button.id].nameChange;
                        if(main.game.server[1]||main.game.client[1]){
                            Pending pending;
                            pending.packet << Packet::Name << button.id << main.player[button.id].name;
                            main.game.queuePacket(pending);
                        }
                    }
                    main.game.nameChange=-1;
                }
                else{
                    main.game.nameChange=button.id;
                    main.game.keyChange[0]=-1;
                    main.player[button.id].nameChange=main.player[button.id].name;
                }
            }
        };
        std::function<void(Main&,ObjectButton<sf::Text>&)> update = [](Main &main, ObjectButton<sf::Text> &button){
            if(main.game.nameChange==button.id){
                button.activated=true;
                button.drawable.setString(main.player[button.id].nameChange); // Edit name
            }
            else{
                button.activated=false;
                button.drawable.setString(main.player[button.id].name); // Edit name
            }
            button.normalColor=main.player[button.id].color;// Set Color
        };
        addToVector2(tempText,visible,action,update,i,sf::Color::Yellow,sf::Text::Italic);
        // Left
        tempText.setString("None2");
        tempText.setFont(font);
        tempText.setCharacterSize(config.fontSize);
        tempText.setColor(sf::Color::Red);
        tempText.setStyle(sf::Text::Regular);
        tempText.setPosition(200,195+(i)*45);
        visible=[](Main &main,ObjectButton<sf::Text> &button){
            if(main.player.size()>button.id && main.player[button.id].local){
                return true;
            }
            return false;
        };
        action = [](Main &main, ObjectButton<sf::Text> &button){
            // Only allow change if local and not ready yet
            if( main.player[button.id].local && (!main.game.client[1] || (main.game.client[1]&&!main.client.ready) ) ){
                // If not active
                if(!button.activated){
                    main.game.keyChange[0]=0; // Key
                    main.game.keyChange[1]=button.id; // Player
                }
                else{
                    main.game.keyChange[0]=-1; // Key
                    //main.game.keyChange[1]=-1; // Player // Not needed
                }
                // Disable name change
                if(main.game.nameChange>-1){
                    main.game.nameChange=-1;
                }
            }
        };
        update = [](Main &main, ObjectButton<sf::Text> &button){
            button.drawable.setString(getKeyName(main.player[button.id].keyL)); // Edit name
            button.normalColor=main.player[button.id].color;// Set Color
            if(main.game.keyChange[1]==button.id && main.game.keyChange[0]==0){
                button.activated=true;
            }
            else{
                button.activated=false;
            }
        };
        addToVector(tempText,visible,action,update,i,sf::Color::Yellow,sf::Text::Italic);
        // Right
        tempText.setString("None3");
        tempText.setFont(font);
        tempText.setCharacterSize(config.fontSize);
        tempText.setColor(sf::Color::Red);
        tempText.setStyle(sf::Text::Regular);
        tempText.setPosition(350,195+(i)*45);
        action = [](Main &main,ObjectButton<sf::Text> &button){
            // Only allow change if local and not ready yet
            if( main.player[button.id].local && (!main.game.client[1] || (main.game.client[1]&&!main.client.ready) ) ){
                // If not active
                if(!button.activated){
                    main.game.keyChange[0]=1; // Key
                    main.game.keyChange[1]=button.id; // Player
                }
                else{
                    main.game.keyChange[0]=-1; // Key
                }
                // Disable name change
                if(main.game.nameChange>-1){
                    main.game.nameChange=-1;
                }
            }
        };
        update = [](Main &main, ObjectButton<sf::Text> &button){
            button.drawable.setString(getKeyName(main.player[button.id].keyR)); // Edit name
            button.normalColor=main.player[button.id].color;// Set Color
            if(main.game.keyChange[1]==button.id && main.game.keyChange[0]==1){
                button.activated=true;
            }
            else{
                button.activated=false;
            }
        };
        addToVector(tempText,visible,action,update,i,sf::Color::Yellow,sf::Text::Italic);
        // Kick
        tempText.setString("Kick");
        tempText.setFont(font);
        tempText.setCharacterSize(config.fontSize);
        tempText.setColor(sf::Color::Red);
        tempText.setStyle(sf::Text::Regular);
        tempText.setPosition(500,195+(i)*45);
        visible=[](Main &main,ObjectButton<sf::Text> &button){
            // Local game: Can kick all. Server: Can kick all. Client: Can kick local players except first
            if(main.player.size()>button.id && ( (main.game.client[1] && button.id!=main.game.id && main.player[button.id].local) || (!main.game.client[1] ) )){
                return true;
            }
            return false;
        };
        action = [](Main &main,ObjectButton<sf::Text> &button){
            // Remove from server
            if(!main.game.client[1]){
                if(main.game.server[1]){
                    if(!main.player[button.id].local){
                        // Remove from clients
                        // Loop needed to find correct one?
                        for(unsigned int j=0;j<main.server.clients[main.player[button.id].id].id.size();j++){
                            if(main.server.clients[main.player[button.id].id].id[j]==button.id){
                                main.server.clients[main.player[button.id].id].id.erase(main.server.clients[main.player[button.id].id].id.begin()+j);
                                break;
                            }
                        }
                        // Kick from server if client has no player left
                        if(main.server.clients[main.player[button.id].id].id.size()==0){
                            main.server.clientMutex.lock();
                            main.server.selector.remove(*main.server.clients[main.player[button.id].id].socket);
                            main.server.clients.erase(main.server.clients.begin()+main.player[button.id].id);
                            main.server.clientMutex.unlock();
                        }
                        // Update player id's
                        main.gameSetup.removePlayer(main.game,main.player,button.id);
                        main.server.updatePlayerID(main.player,button.id);
                    }
                    else{
                        main.gameSetup.removePlayer(main.game,main.player,button.id);
                    }
                    //
                    Pending pending;
                    pending.packet << Packet::Disconnect << button.id;
                    main.game.queuePacket(pending);
                }
                else{
                    main.gameSetup.removePlayer(main.game,main.player,button.id);
                }
            }
            // Send to server
            else{
                main.player[button.id].local=false; // Needed why?
                Pending pending;
                pending.packet << Packet::RemovePlayer << button.id;
                main.game.queuePacket(pending);
            }
        };
        update = [](Main &main, ObjectButton<sf::Text> &button){
            button.normalColor=main.player[button.id].color;// Set Color
        };
        addToVector(tempText,visible,action,update,i,sf::Color::Yellow,sf::Text::Italic);
        // Status
        tempText.setString("Status");
        tempText.setFont(font);
        tempText.setCharacterSize(config.fontSize);
        tempText.setColor(sf::Color::Red);
        tempText.setStyle(sf::Text::Regular);
        tempText.setPosition(200,195+(i)*45);
        std::function<bool(Main&, ObjectStatic<sf::Text>&)> visible2=[](Main &main,ObjectStatic<sf::Text> &button){
            if(main.player.size()>button.id && !main.player[button.id].local){
                return true;
            }
            return false;
        };
        std::function<void(Main&, ObjectStatic<sf::Text>&)> update2 = [](Main &main, ObjectStatic<sf::Text> &button){
            button.drawable.setColor(main.player[button.id].color);// Set Color
            if( main.player[button.id].server ){
                button.drawable.setString("Server");
            }
            else if( main.game.server[1] ){
                if( !main.server.clients[main.player[button.id].id].ready ){
                    button.drawable.setString("Not ready: " + std::to_string(main.player[button.id].id) );
                }
                else{
                    button.drawable.setString("Ready: " + std::to_string(main.player[button.id].id) );
                }
            }
            else{
                if( !main.player[button.id].ready ){
                    button.drawable.setString("Not ready");
                }
                else{
                    button.drawable.setString("Ready");
                }
            }
        };
        addToVector(tempText,visible2,update2,id);
    }
    // Options
    tempText.setString("Options");
    tempText.setFont(font);
    tempText.setCharacterSize(config.fontSmallSize);
    tempText.setColor(sf::Color::White);
    tempText.setStyle(sf::Text::Bold);
    tempText.setPosition(750 , 150);
    addToVector(tempText);
    //
    tempText.setString("Max Points:");
    tempText.setFont(font);
    tempText.setCharacterSize(config.fontSmallSize);
    tempText.setColor(sf::Color::White);
    tempText.setStyle(sf::Text::Regular);
    tempText.setPosition(670 , 200);
    addToVector(tempText);
    //
    tempText.setString("10");
    tempText.setFont(font);
    tempText.setCharacterSize(config.fontSmallSize);
    tempText.setColor(sf::Color::Red);
    tempText.setStyle(sf::Text::Regular);
    tempText.setPosition(840 , 200);
    action = [](Main &main, ObjectButton<sf::Text> &button){
        if(!main.game.client[1]){
            main.game.maxPoints=10;
            main.game.optionsChanged();
        }
    };
    std::function<void(Main&, ObjectButton<sf::Text>&)> update = [](Main &main, ObjectButton<sf::Text> &button){
        if(main.game.maxPoints==10){
            button.activated=true;
        }
        else{
            button.activated=false;
        }
    };
    unsigned int damn=0;
    addToVector(tempText,trueFunction,action,update,damn,sf::Color::Yellow,sf::Text::Italic);
    //
    tempText.setString("20");
    tempText.setFont(font);
    tempText.setCharacterSize(config.fontSmallSize);
    tempText.setColor(sf::Color::Red);
    tempText.setStyle(sf::Text::Regular);
    tempText.setPosition(880 , 200);
    action = [](Main &main, ObjectButton<sf::Text> &button){
        if(!main.game.client[1]){
            main.game.maxPoints=20;
            main.game.optionsChanged();
        }
    };
    update = [](Main &main, ObjectButton<sf::Text> &button){
        if(main.game.maxPoints==20){
            button.activated=true;
        }
        else{
            button.activated=false;
        }
    };
    addToVector(tempText,trueFunction,action,update,damn,sf::Color::Yellow,sf::Text::Italic);
    //
    tempText.setString("40");
    tempText.setFont(font);
    tempText.setCharacterSize(config.fontSmallSize);
    tempText.setColor(sf::Color::Red);
    tempText.setStyle(sf::Text::Regular);
    tempText.setPosition(920 , 200);
    action = [](Main &main, ObjectButton<sf::Text> &button){
        if(!main.game.client[1]){
            main.game.maxPoints=40;
            main.game.optionsChanged();
        }
    };
    update = [](Main &main, ObjectButton<sf::Text> &button){
        if(main.game.maxPoints==40){
            button.activated=true;
        }
        else{
            button.activated=false;
        }
    };
    addToVector(tempText,trueFunction,action,update,damn,sf::Color::Yellow,sf::Text::Italic);
    //
    tempText.setString("Powerups:");
    tempText.setFont(font);
    tempText.setCharacterSize(config.fontSmallSize);
    tempText.setColor(sf::Color::White);
    tempText.setStyle(sf::Text::Regular);
    tempText.setPosition(670 , 240);
    addToVector(tempText);
    //
    tempText.setString("On");
    tempText.setFont(font);
    tempText.setCharacterSize(config.fontSmallSize);
    tempText.setColor(sf::Color::Red);
    tempText.setStyle(sf::Text::Regular);
    tempText.setPosition(840, 240);
    action = [](Main &main, ObjectButton<sf::Text> &button){
        if(!main.game.client[1]){
            main.game.powerupEnabled=true;
            main.game.optionsChanged();
        }
    };
    update = [](Main &main, ObjectButton<sf::Text> &button){
        if(main.game.powerupEnabled){
            button.activated=true;
        }
        else{
            button.activated=false;
        }
    };
    addToVector(tempText,trueFunction,action,update,damn,sf::Color::Yellow,sf::Text::Italic);
    //
    tempText.setString("Off");
    tempText.setFont(font);
    tempText.setCharacterSize(config.fontSmallSize);
    tempText.setColor(sf::Color::Red);
    tempText.setStyle(sf::Text::Regular);
    tempText.setPosition(890, 240);
    action = [](Main &main, ObjectButton<sf::Text> &button){
        if(!main.game.client[1]){
            main.game.powerupEnabled=false;
            main.game.optionsChanged();
        }
    };
    update = [](Main &main, ObjectButton<sf::Text> &button){
        if(!main.game.powerupEnabled){
            button.activated=true;
        }
        else{
            button.activated=false;
        }
    };
    addToVector(tempText,trueFunction,action,update,damn,sf::Color::Yellow,sf::Text::Italic);
    //
    tempText.setString("Countdown:");
    tempText.setFont(font);
    tempText.setCharacterSize(config.fontSmallSize);
    tempText.setColor(sf::Color::White);
    tempText.setStyle(sf::Text::Regular);
    tempText.setPosition(670 , 280);
    std::function<bool(Main&, ObjectStatic<sf::Text>&)> visible2=[](Main &main,ObjectStatic<sf::Text> &button){
        if(!main.game.server[1]&&!main.game.client[1]){
            return true;
        }
        return false;
    };
    addToVector(tempText,visible2);
    //
    tempText.setString("On");
    tempText.setFont(font);
    tempText.setCharacterSize(config.fontSmallSize);
    tempText.setColor(sf::Color::Red);
    tempText.setStyle(sf::Text::Regular);
    tempText.setPosition(840, 280);
    visible=[](Main &main,ObjectButton<sf::Text> &button){
        if(!main.game.server[1]&&!main.game.client[1]){
            return true;
        }
        return false;
    };
    action = [](Main &main, ObjectButton<sf::Text> &button){
        if(!main.game.client[1]){
            main.game.countdownEnabled=true;
            main.game.optionsChanged();
        }
    };
    update = [](Main &main, ObjectButton<sf::Text> &button){
        if(main.game.countdownEnabled){
            button.activated=true;
        }
        else{
            button.activated=false;
        }
    };
    addToVector(tempText,visible,action,update,damn,sf::Color::Yellow,sf::Text::Italic);
    //
    tempText.setString("Off");
    tempText.setFont(font);
    tempText.setCharacterSize(config.fontSmallSize);
    tempText.setColor(sf::Color::Red);
    tempText.setStyle(sf::Text::Regular);
    tempText.setPosition(890, 280);
    action = [](Main &main, ObjectButton<sf::Text> &button){
        if(!main.game.client[1]){
            main.game.countdownEnabled=false;
            main.game.optionsChanged();
        }
    };
    update = [](Main &main, ObjectButton<sf::Text> &button){
        if(!main.game.countdownEnabled){
            button.activated=true;
        }
        else{
            button.activated=false;
        }
    };
    addToVector(tempText,visible,action,update,damn,sf::Color::Yellow,sf::Text::Italic);
    //
    tempText.setString("Multiple players:");
    tempText.setFont(font);
    tempText.setCharacterSize(config.fontSmallSize);
    tempText.setColor(sf::Color::White);
    tempText.setStyle(sf::Text::Regular);
    tempText.setPosition(670 , 280);
    visible2=[](Main &main,ObjectStatic<sf::Text> &button){
        if(main.game.server[1] || main.game.client[1]){
            return true;
        }
        return false;
    };
    addToVector(tempText,visible2);
    //
    tempText.setString("On");
    tempText.setFont(font);
    tempText.setCharacterSize(config.fontSmallSize);
    tempText.setColor(sf::Color::Red);
    tempText.setStyle(sf::Text::Regular);
    tempText.setPosition(840, 280);
    visible=[](Main &main,ObjectButton<sf::Text> &button){
        if(main.game.server[1]||main.game.client[1]){
            return true;
        }
        return false;
    };
    action = [](Main &main, ObjectButton<sf::Text> &button){
        if(!main.game.client[1]){
            main.game.multiplePlayersEnabled=true;
            main.game.optionsChanged();
        }
    };
    update = [](Main &main, ObjectButton<sf::Text> &button){
        if(main.game.multiplePlayersEnabled){
            button.activated=true;
        }
        else{
            button.activated=false;
        }
    };
    addToVector(tempText,visible,action,update,damn,sf::Color::Yellow,sf::Text::Italic);
    //
    tempText.setString("Off");
    tempText.setFont(font);
    tempText.setCharacterSize(config.fontSmallSize);
    tempText.setColor(sf::Color::Red);
    tempText.setStyle(sf::Text::Regular);
    tempText.setPosition(890, 280);
    action = [](Main &main, ObjectButton<sf::Text> &button){
        if(!main.game.client[1]){
            main.game.multiplePlayersEnabled=false;
            main.game.optionsChanged();
        }
    };
    update = [](Main &main, ObjectButton<sf::Text> &button){
        if(!main.game.multiplePlayersEnabled){
            button.activated=true;
        }
        else{
            button.activated=false;
        }
    };
    addToVector(tempText,visible,action,update,damn,sf::Color::Yellow,sf::Text::Italic);
    // Server info
    tempText.setString("Start server");
    tempText.setFont(font);
    tempText.setCharacterSize(config.fontSize);
    tempText.setColor(sf::Color::Red);
    tempText.setStyle(sf::Text::Regular);
    tempText.setPosition(40,config.windowHeight-50);
    action = [](Main &main, ObjectButton<sf::Text> &button){
        // Depending on situation: start Server, Stop Server or do nothing.
        if(main.game.client[1]){
            return;
        }
        else if(!main.game.server[1]){
            main.server.start(main.config,main.gameSetup,main.game,main.player);
        }
        else{
            main.server.shutdown(main.game);
        }
    };
    update = [](Main &main, ObjectButton<sf::Text> &button){
        if(main.game.server[1]){
            button.drawable.setString("Local: " + sf::IpAddress::getLocalAddress().toString());
        }
        else if(main.game.client[1]){
            button.drawable.setString("Server: " + main.game.serverIp);
        }
        else{
            button.drawable.setString("Start Server");
        }
    };
    addToVector(tempText,trueFunction,action,update,damn,sf::Color::Yellow,sf::Text::Italic);
}
//
void ObjectStorage::initGame(const Config &config, Game &game, std::vector<Player> &player){
    objects.clear();
    mode=Game::Mode::Play;
    // Walls
    std::function<bool(Main&, ObjectStatic<sf::RectangleShape>&)> trueFunction=[](Main &main,ObjectStatic<sf::RectangleShape> &button){return true;};
    std::function<void(Main&, ObjectStatic<sf::RectangleShape>&)> wallUpdate=[](Main &main,ObjectStatic<sf::RectangleShape> &wall){
        sf::Color color=main.config.wallColor;
        if(main.game.wallsAway){
            color.a= pow(cos( (float) (main.game.frame % (main.config.fps*2) )/ main.config.fps * (PI)  ),2) * 255;
        }
        wall.drawable.setFillColor(color);
    };
    sf::RectangleShape wall;
    // Left
    wall.setSize(sf::Vector2f(config.wallWidth, config.windowHeight));
    addToVector(wall,trueFunction,wallUpdate);
    // Top
    wall.setSize(sf::Vector2f(config.windowWidth-config.statusWidth-2*config.wallWidth,config.wallWidth));
    wall.setPosition(config.wallWidth,0);
    addToVector(wall,trueFunction,wallUpdate);
    // Right
    wall.setSize(sf::Vector2f(config.wallWidth, config.windowHeight));
    wall.setPosition(config.windowWidth-config.wallWidth-config.statusWidth,0);
    addToVector(wall,trueFunction,wallUpdate);
    // Bottom
    wall.setSize(sf::Vector2f(config.windowWidth-config.statusWidth-2*config.wallWidth,config.wallWidth));
    wall.setPosition(config.wallWidth,config.windowHeight-config.wallWidth);
    addToVector(wall,trueFunction,wallUpdate);
    // Score Title
    sf::Text temp;
    temp.setFont(font);
    temp.setString("Scores");
    temp.setCharacterSize(config.fontSize);
    temp.setPosition( config.windowWidth-config.statusWidth/2-temp.getLocalBounds().width/2,10);
    temp.setColor(sf::Color::White);
    temp.setStyle(sf::Text::Bold);
    addToVector(temp);
    // Round
    temp.setCharacterSize(config.fontSmallSize);
    temp.setFont(font);
    temp.setColor(sf::Color::Yellow);
    temp.setString("Round:");
    temp.setPosition(config.windowWidth-config.statusWidth+5,config.windowHeight/2+30);
    addToVector(temp);
    temp.setCharacterSize(config.fontSmallSize);
    temp.setFont(font);
    temp.setColor(sf::Color::Yellow);
    temp.setPosition(config.windowWidth-config.statusWidth+100,config.windowHeight/2+30);
    std::function<void(Main&, ObjectStatic<sf::Text>&)> roundUpdate=[](Main &main,ObjectStatic<sf::Text> &text){
        text.drawable.setString(std::to_string(main.game.round));
    };
    std::function<bool(Main&, ObjectStatic<sf::Text>&)> trueFunction2=[](Main &main,ObjectStatic<sf::Text> &button){return true;};
    addToVector(temp,trueFunction2,roundUpdate);
    // FPS
    temp.setCharacterSize(config.fontSmallSize);
    temp.setFont(font);
    temp.setColor(sf::Color::Yellow);
    temp.setString("FPS:");
    temp.setPosition(config.windowWidth-config.statusWidth+5,config.windowHeight/2+60);
    addToVector(temp);
    temp.setCharacterSize(config.fontSmallSize);
    temp.setFont(font);
    temp.setColor(sf::Color::Yellow);
    temp.setPosition(config.windowWidth-config.statusWidth+100,config.windowHeight/2+60);
    std::function<void(Main&, ObjectStatic<sf::Text>&)> fpsUpdate=[](Main &main,ObjectStatic<sf::Text> &text){
        text.drawable.setString(std::to_string(main.renderer.fps));
    };
    addToVector(temp,trueFunction2,fpsUpdate);
    // gameFPS
    temp.setCharacterSize(config.fontSmallSize);
    temp.setFont(font);
    temp.setColor(sf::Color::Yellow);
    temp.setString("GPS:");
    temp.setPosition(config.windowWidth-config.statusWidth+5,config.windowHeight/2+90);
    addToVector(temp);
    temp.setCharacterSize(config.fontSmallSize);
    temp.setFont(font);
    temp.setColor(sf::Color::Yellow);
    temp.setPosition(config.windowWidth-config.statusWidth+100,config.windowHeight/2+90);
    std::function<void(Main&, ObjectStatic<sf::Text>&)> gamefpsUpdate=[](Main &main,ObjectStatic<sf::Text> &text){
        text.drawable.setString(std::to_string(main.game.frameTime));
    };
    addToVector(temp,trueFunction2,gamefpsUpdate);
    // Packet
    temp.setCharacterSize(config.fontSmallSize);
    temp.setFont(font);
    temp.setColor(sf::Color::Yellow);
    temp.setString("PPS:");
    temp.setPosition(config.windowWidth-config.statusWidth+5,config.windowHeight/2+120);
    std::function<bool(Main&, ObjectStatic<sf::Text>&)> ppsVisible=[](Main &main,ObjectStatic<sf::Text> &button){
        if(main.game.client[1] || main.game.server[1]){
            return true;
        }
        return false;
    };
    addToVector(temp,ppsVisible);
    temp.setCharacterSize(config.fontSmallSize);
    temp.setFont(font);
    temp.setColor(sf::Color::Yellow);
    temp.setPosition(config.windowWidth-config.statusWidth+100,config.windowHeight/2+120);
    std::function<void(Main&, ObjectStatic<sf::Text>&)> packetUpdate=[](Main &main,ObjectStatic<sf::Text> &text){
        text.drawable.setString(std::to_string( static_cast<int>(main.config.gameUpdateThreadMinRate/main.game.packetTime) ));
    };
    addToVector(temp,ppsVisible,packetUpdate);
    // Quit Button
    temp.setCharacterSize(config.fontSize);
    temp.setStyle(sf::Text::Bold);
    temp.setFont(font);
    temp.setColor(sf::Color::Red);
    temp.setString("Quit");
    temp.setPosition(config.windowWidth-config.statusWidth/2-temp.getLocalBounds().width/2,config.windowHeight-50);
    std::function<void(Main&, ObjectButton<sf::Text>&)> action=[](Main &main,ObjectButton<sf::Text> &text){
        main.game.quit(main.config);
        if(main.game.mode==Game::Mode::mainMenu){
            main.storage.initMainMenu(main.config);
        }
        else{
            main.storage.initGameSetup(main.config);
        }
    };
    std::function<bool(Main&, ObjectButton<sf::Text>&)> trueFunction3=[](Main &main,ObjectButton<sf::Text> &button){return true;};
    addToVector(temp,trueFunction3,action,sf::Color::Yellow,sf::Text::Italic|sf::Text::Bold);
    // End round message
    temp.setStyle(sf::Text::Bold);
    temp.setCharacterSize(50);
    temp.setFont(font);
    std::function<void(Main&, ObjectStatic<sf::Text>&)> endRoundUpdate=[](Main &main,ObjectStatic<sf::Text> &text){
        if(main.game.roundFinished){
            if(main.game.gameFinished){
                text.drawable.setString(main.player[main.game.roundWinner].name + " has won the game!");
                text.drawable.setColor(main.player[main.game.roundWinner].color);
            }
            else if(main.game.roundWinner==-1){
                text.drawable.setString("Round finished!");
                text.drawable.setColor(sf::Color::Red);
            }
            else{
                text.drawable.setString(main.player[main.game.roundWinner].name + " has won the round!");
                text.drawable.setColor(main.player[main.game.roundWinner].color);
            }
            text.drawable.setPosition(main.config.windowWidth/2-main.config.statusWidth/2-text.drawable.getLocalBounds().width/2,main.config.windowHeight/2-text.drawable.getLocalBounds().height/2);
        }
    };
    std::function<bool(Main&, ObjectStatic<sf::Text>&)> visible = [](Main &main,ObjectStatic<sf::Text> &text){
      if(main.game.roundFinished){
        return true;
      }
      return false;
    };
    addToVector(temp,visible,endRoundUpdate);
    // Countdown
    temp.setStyle(sf::Text::Bold);
    temp.setCharacterSize(50);
    temp.setFont(font);
    std::function<void(Main&, ObjectStatic<sf::Text>&)> countdownUpdate=[](Main &main,ObjectStatic<sf::Text> &text){
        if(main.game.countdownInt>0){
            text.drawable.setString(std::to_string(main.game.countdownInt));
            if(main.game.countdownInt==3){
                text.drawable.setColor(sf::Color::Red);
            }
            else if(main.game.countdownInt==2){
                text.drawable.setColor(sf::Color::Yellow);
            }
            else{
                text.drawable.setColor(sf::Color::Green);
            }
            text.drawable.setPosition(main.config.windowWidth/2-main.config.statusWidth/2-text.drawable.getLocalBounds().width/2,main.config.windowHeight/2-text.drawable.getLocalBounds().height/2);
        }
    };
    visible = [](Main &main,ObjectStatic<sf::Text> &text){
      if(main.game.countdownInt>0){
        return true;
      }
      return false;
    };
    addToVector(temp,visible,countdownUpdate);
    // Player things
    for(unsigned int i=0;i<player.size();i++){
        //
        temp.setString("Player");
        temp.setFont(font);
        temp.setCharacterSize(config.fontSmallSize);
        temp.setStyle(sf::Text::Regular);
        temp.setPosition(config.windowWidth-config.statusWidth+5,50+i*30);
        std::function<bool(Main&, ObjectStatic<sf::Text>&)> visible = [](Main &main,ObjectStatic<sf::Text> &button){
            if(main.player.size()>button.id){
                return true;
            }
            return false;
        };
        std::function<void(Main&, ObjectStatic<sf::Text>&)> nameUpdate = [](Main &main, ObjectStatic<sf::Text> &button){
            button.drawable.setString(main.player[button.id].name);
            button.drawable.setColor(main.player[button.id].color);
        };
        addToVector(temp, visible, nameUpdate, i);
        //
        temp.setPosition(config.windowWidth-30,50+i*30);
        std::function<void(Main&,ObjectStatic<sf::Text>&)> scoreUpdate = [](Main &main, ObjectStatic<sf::Text> &button){
            button.drawable.setString(std::to_string(main.player[button.id].points));
            button.drawable.setColor(main.player[button.id].color);
        };
        addToVector(temp, visible, scoreUpdate, i);
        // Player lines
        std::unique_ptr<ObjectLine> temp(new ObjectLine(&player[i].line, &player[i].noTurtleLine, i, &player, &game.darkness, config ) );
        objects.push_back(std::move(temp));
        // Head
        std::unique_ptr<ObjectHead> temp3(new ObjectHead(&player[i].circle, &player[i].rectangle, &player[i].death, &player[i].rightAngle, i ) );
        objects.push_back(std::move(temp3));
    }
    // Powerup time
    std::unique_ptr<ObjectPowerups> temp4(new ObjectPowerups( config, &game.powerupField, &player, &game.darkness ) );
    objects.push_back(std::move(temp4));
}
// Font Loader; Funny that I never catch anything
sf::Font ObjectStorage::loadFont(const std::string& name){
    HRSRC rsrcData = FindResource(NULL, name.c_str(), RT_RCDATA);
    if (!rsrcData)
        throw std::runtime_error("Failed to find resource.");

    DWORD rsrcDataSize = SizeofResource(NULL, rsrcData);
    if (rsrcDataSize <= 0)
        throw std::runtime_error("Size of resource is 0.");

    HGLOBAL grsrcData = LoadResource(NULL, rsrcData);
    if (!grsrcData)
        throw std::runtime_error("Failed to load resource.");

    LPVOID firstByte = LockResource(grsrcData);
    if (!firstByte)
        throw std::runtime_error("Failed to lock resource.");

    sf::Font font;
    if (!font.loadFromMemory(firstByte, rsrcDataSize))
        throw std::runtime_error("Failed to load font from memory.");

    return font;
}
//
template<class T>
void ObjectStorage::addToVector(T &object){
    // Create
    std::function<bool(Main&,ObjectStatic<T>&)> trueFunction=[](Main &main,ObjectStatic<T> &button){return true;};
    std::unique_ptr<ObjectStatic<T>> temp(new ObjectStatic<T>( object, std::move(trueFunction) ) );
    // Push back
    objects.push_back(std::move(temp));
}
//
template<class T>
void ObjectStorage::addToVector(T &object,std::function<bool(Main&,ObjectStatic<T>&)> &visible){
    std::unique_ptr<ObjectStatic<T>> temp(new ObjectStatic<T>( object, std::move(visible) ) );
    // Push back
    objects.push_back(std::move(temp));
}
//
template<class T>
void ObjectStorage::addToVector(T &object,std::function<bool(Main&,ObjectStatic<T>&)> &visible,std::function<void(Main&,ObjectStatic<T>&)> &update){
    // Create
    std::unique_ptr<ObjectStatic<T>> temp(new ObjectStatic<T>( object, std::move(visible), std::move(update) ) );
    // Push back
    objects.push_back(std::move(temp));
}
template<class T>
void ObjectStorage::addToVector(T &object,std::function<bool(Main&,ObjectStatic<T>&)> &visible,std::function<void(Main&,ObjectStatic<T>&)> &update, unsigned int &id){
    // Create
    std::unique_ptr<ObjectStatic<T>> temp(new ObjectStatic<T>( object, std::move(visible), std::move(update), id ) );
    // Push back
    objects.push_back(std::move(temp));
}
//
template<class T>
void ObjectStorage::addToVector(T &object, std::function<bool(Main&,ObjectButton<T>&)> &visible, std::function<void(Main&,ObjectButton<T>&)> &action,const  sf::Color &selectedColor,const unsigned int &selectedStyle){
    // Create
    std::unique_ptr<ObjectButton<T>> temp(new ObjectButton<T>( std::move(object), std::move(visible), std::move(action),selectedColor, selectedStyle)) ;
    // Push back
    objects.push_back(std::move(temp));
}
template<class T>
void ObjectStorage::addToVector(T object, std::function<bool(Main&,ObjectButton<T>&)> &visible, std::function<void(Main&,ObjectButton<T>&)> &action, std::function<void(Main&,ObjectButton<T>&)> &update, unsigned int &id, const sf::Color &selectedColor,const unsigned int &selectedStyle){
    // Create
    std::unique_ptr<ObjectButton<T>> temp(new ObjectButton<T>( std::move(object), std::move(visible), std::move(action),std::move(update), id, selectedColor, selectedStyle)) ;
    // Push back
    objects.push_back(std::move(temp));
}
void ObjectStorage::addToVector2(sf::Text object, std::function<bool(Main&,ObjectButton<sf::Text>&)> &visible, std::function<void(Main&,ObjectButton<sf::Text>&)> &action, std::function<void(Main&,ObjectButton<sf::Text>&)> &update, unsigned int &id,const sf::Color &selectedColor,const unsigned int &selectedStyle){
    // Create
    std::unique_ptr<TextBox> temp(new TextBox( std::move(object), std::move(visible), std::move(action),std::move(update), id, selectedColor, selectedStyle)) ;
    // Push back
    objects.push_back(std::move(temp));
}
