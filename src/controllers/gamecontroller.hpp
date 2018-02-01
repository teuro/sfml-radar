#ifndef GAMECONTROLLER_HPP
#define GAMECONTROLLER_HPP

#include <cstdlib>

#include "../models/game.hpp"
#include "../models/weather/atis.hpp"
#include "../models/weather/metar.hpp"
#include "../models/menu/menu.hpp"
#include "../models/menu/menu_item.hpp"
#include "../tools/tools.hpp"
#include "../tools/point_tools.hpp"
#include "../tools/time_tools.hpp"
#include "../tools/text_tools.hpp"
#include "../views/game/game_view.hpp"
#include "../views/atis/atis_view.hpp"
#include "../views/stat/stat_view.hpp"
#include "../views/menu/menu_view.hpp"
#include "controller.hpp"

/**
	* Gamecontroller
	* Responsible for linking gameview and model together
**/

class Gamecontroller : public Controller {
public:
	/**
		* Constructor
		* @param Settings&
		* @param Drawsurface&
	**/
    Gamecontroller(Settings& s, Drawsurface& d);
    ~Gamecontroller();
	/**
		* handle_function_keys
		* reacts any high-level buttons
		* @param int key
		* @return void
	**/
	
    std::string handle_function_keys();
	
	/**
		* handle_mouse_release
		* Moves center point 
		* @param Point mouse_start point where mouse left button was pushed
		* @param Point mouse_end point where mouse left button was released
		* @return void
	**/
    void handle_mouse_release(Point& mouse_start, Point& mouse_end);
	
	/**
		* handle_mouse_wheel
		* Sets new zoom level for game 
		* @param int amount
		* @return void
	**/
	
    void handle_mouse_wheel(int amount);
	
	/**
		* set_centerpoint
		* Sets new centerpoint for settings
		* @param Coordinbate&
		* @return void
	**/
	
    void set_centerpoint(Coordinate& cp);
	
	/**
		* get_centerpoint
		* get centerpoint from settings
		* @param void
		* @return Coordinate&
	**/
	
    Coordinate& get_centerpoint();
	
	/**
		* update
		* Moves forward by given amount of milliseconds
		* @param double elapsed
		* @param Point& mouse
		* @return void
	**/
	
    void update(double elapsed, Point& mouse);
	
	/**
		* handle_mouse_click
		* Selects a item from screen
		* @param Point& mouse
		* @return void
	**/
	
    void handle_mouse_click(Point& mouse);
	
	/**
		* load
		* Loads needed values and initializing everything before use
		* @param void
		* @return void
	**/
	
    void load();
	
	/**
		* handle_text_input
		* Parses command and passes appropriate class forward. Do nothing visible
		* @param void
		* @return void
	**/
	
    void handle_text_input();
	
	/**
		* update_command
		* Receive from program. Moves to internal variable. Do nothing visble
		* @param std::string command
		* @return void
	**/
	
    void update_command(std::string command);
	
	/**
		* is_ok
		* @todo Do checks if everything is ok and game shall end.
		* @param void
		* @return bool
	**/
	
    bool is_ok();
	
protected:
private:
	void set_variables();
	void calculate_fps();
	void load_menu_items(std::string query, Menu*& menu);

	std::list <Clearance> clearances;
	std::list <std::string> quicklist;
	std::list <std::string> :: iterator quick_iterator;
	std::list <std::string> matching_elements(std::string input);
	
    std::shared_ptr <Gameview> gameview;
    std::shared_ptr <Atisview> atisview;
    std::shared_ptr <Statview> statview;
    std::shared_ptr <Menuview> menuview;

    std::shared_ptr <Game> game;
	std::shared_ptr <Atis> atis;

	Metar metar;

	Menu* menu;
	Menu* airports;

	int function_key;
	double game_time;
	double flash_message_begin;
	double flash_message_time;
	std::string message;
	enum states {MENU, ATIS, GAME, STAT};
	int state;
	void set_flash_message(std::string message);
};

#endif // GAMECONTROLLER_HPP
