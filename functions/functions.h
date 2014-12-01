// All functions that operate outside of classes
#ifndef CURVE_FUNCTIONS
#define CURVE_FUNCTIONS

// Needed Headers
//#include <SFML/Graphics.hpp>
#include "../curve.h"
// Convention: Config &config, Game &game, Player player[], sf::RenderWindow &window, other
// sf::Event event

// Renderer.cpp
/*void Render_Thread(const Config &config,Game &game,Player player[],sf::RenderWindow &window);
void Render_Main_Menu(const Config &config,Game &game,Player player[],sf::RenderWindow &window,Main_Menu &main_menu);
void Render_Server_IP(const Config &config,Game &game,Player player[],sf::RenderWindow &window,Main_Menu &main_menu);
void Render_Setup_SP(const Config &config,Game &game,Player player[],sf::RenderWindow &window,Game_Setup &game_setup);
void Render_Setup_MP(const Config &config,Game &game,Player player[],sf::RenderWindow &window,Game_Setup_MP &game_setup);
void Render_Game(const Config &config,Game &game,Player player[],sf::RenderWindow &window,Game_Store &game_store);
void Render_Game_MP(const Config &config,Game &game,Player player[],sf::RenderWindow &window,Game_Store &game_store);
void Render_PowerUp(const Config &config,Game &game,sf::RenderWindow &window,Powerup_Sprite &sprite);
void Render_Game_Status_Menu(const Config &config,Game &game, Player player[],sf::RenderWindow &window,Game_Store &game_store);*/

// event_handler.cpp
void Event_Handler(Config &config,Game &game,Player player[],sf::RenderWindow &window,sf::Event &event);
void Pause_Game(const Config &config,Game &game,bool pause);

// game_setup.cpp
void Initialize_Game_Setup(const Config &config,Game &game,Player player[]);
void Initialize_Game_Setup_MP(const Config &config,Game &game,Player player[]);
void Switch_Player(Game &game,Player player[],int i);
bool Key_Available(Player player[],sf::Keyboard::Key key);
void Auto_Add_Players(Player player[]);

// game.cpp
void Initialize_Game(const Config &config,Game &game,Player player[]);
void Initialize_Game_Client(const Config &config,Game &game,Player player[]);
void Initialize_New_Round(const Config &config,Game &game,Player player[]);
void Initialize_New_Round_Server(const Config &config,Game &game,Player player[]);
void Initialize_New_Round_Client(const Config &config,Game &game,Player player[]);
void Start_Game_Update(const Config &config,Game &game,Player player[]);
void Game_Update_Thread(const Config &config,Game &game,Player player[]);
void Game_Update_Thread_Server(const Config &config,Game &game,Player player[]);
void Game_Update_Thread_Client(const Config &config,Game &game,Player player[]);
void Hit_Detector(const Config &config,Game &game,Player player[]);
void Player_Death(Game &game,Player player[],std::vector<int> &death_vec);
void Add_Points(Player player[], std::vector<int> &death_vec);
void Add_Points_Server(Game &game,Player player[], std::vector<int> &death_vec);
void End_Round(const Config &config,Game &game,Player player[]);
void PowerUp_Manager(const Config &config,Game &game,Player player[]);
void PowerUp_Manager_Client(const Config &config,Game &game);

// server.cpp
//void Start_Server(const Config &config,Game &game,Player player[],Server &server);
//void Server_Listener(const Config &config, Game &game,Player player[],Server &server);
//void Server_Sender(const Config &config, Game &game,Player player[],Server &server);
//void Client_Init_Packages(const Config &config,Game &game,Player player[],Server &server);
void Server_Sync_Clients(const Config &config,Game &game,Player player[]);
void Server_Start_Game(const Config &config,Game &game,Player player[]);
void Config_Package(Game &game);
//void Server_Process_Package(const Config &config,Game &game,Player player[],Server &server,sf::Packet &packet);

// client.cpp
/*void Start_Client(const Config &config, Game &game,Player player[]);
void Client_Thread(const Config &config,Game &game,Player player[]);
void Client_Ready(Game &game,Player player[]);
void Client_Process_Packet(const Config &config,Game &game,Player player[],sf::Packet &packet);*/

// Temporary
void Client_Ready(Game &game,Player player[]);

// General.cpp
const char *getKeyName( const sf::Keyboard::Key key );
sf::String int2string(int x);
void Sleep(const int &frame_time,sf::Clock &clock);
sf::Image LoadImageFromResource(const std::string& name);
sf::Font LoadFont(const std::string& name);

#endif // CURVE_FUNCTIONS
