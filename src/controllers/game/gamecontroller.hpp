
#ifndef GAMECONTROLLER_HPP
#define GAMECONTROLLER_HPP

#include <cstdlib>
#include <fstream>
#include <iostream>

#include "../../models/game.hpp"
#include "../../models/weather/atis.hpp"
#include "../../models/weather/metar.hpp"
#include "../../models/menu/menu.hpp"
#include "../../models/menu/menu_item.hpp"
#include "../../tools/point_tools.hpp"
#include <tools/tools.hpp>
#include <tools/time_tools.hpp>
#include <tools/text_tools.hpp>
#include "../../views/game/game_view.hpp"
#include "../../views/atis/atis_view.hpp"
#include "../../views/stat/stat_view.hpp"
#include "../../views/menu/menu_view.hpp"
#include "../controller.hpp"

/**
	* Gamecontroller
	* Responsible for linking views and models together
**/

class Gamecontroller : public Controller {
public:
	/**
		* Constructor
		* @param Settings&
		* @param Drawsurface&
	**/
    Gamecontroller(std::shared_ptr <Settings> s, Drawsurface& d);
	
    ~Gamecontroller();
	/**
		* handle_function_keys
		* reacts any high-level buttons
		* @param int key
		* @return void
	**/
	
    void handle_function_keys(int key) override;
	
	/**
		* handle_mouse_release
		* Moves center point 
		* @param Point mouse_start point where mouse left button was pushed
		* @param Point mouse_end point where mouse left button was released
		* @return void
	**/
    void handle_mouse_release(Point& mouse_start, Point& mouse_end) override;
	
	/**
		* handle_mouse_wheel
		* Sets new zoom level for game 
		* @param int amount
		* @return void
	**/
	
    void handle_mouse_wheel(int amount) override;
	
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
	
    void update(double elapsed) override;
	
	/**
		* handle_mouse_click
		* Selects a item from screen
		* @param Point& mouse
		* @return void
	**/
	
    void handle_mouse_click(Point& mouse) override;
	
	/**
		* load
		* Loads needed values and initializing everything before use
		* @param void
		* @return void
	**/
	
    void load() override;
	
	/**
		* handle_text_input
		* Parses command and passes appropriate class forward. Do nothing visible
		* @param void
		* @return void
	**/
	
    void handle_text_input() override;
	
	/**
		* update_command
		* Receive from program. Moves to internal variable. Do nothing visble
		* @param std::string command
		* @return void
	**/
	
    void update_command(std::string command) override;
	
	void draw(Point& mouse);
private:
	bool level_selected;
	bool airport_selected;

	/**
		* set_variables
		* Set game variables for views.
		* @param void
		* @return void
	**/
	void set_variables();
	
	/**
		* calculate_fps
		* Calculates fps and stores to proper variable
		* @param void
		* @return void
	**/
	void calculate_fps();
	
	/**
		* load_menu_items
		* loads data form menu
		* @param query
		* @param menu
	**/
	void load_menu_items(std::string query, std::shared_ptr <Menu> menu);
	
	std::list <std::string> quicklist;
	std::list <std::string> :: iterator quick_iterator;
	std::list <std::string> matching_elements(std::string input);
	
    std::shared_ptr <Gameview> gameview;
    std::shared_ptr <Atisview> atisview;
    std::shared_ptr <Statview> statview;
    std::shared_ptr <Menuview> menuview;
	
	std::map <int, std::shared_ptr <View> > views;

    std::shared_ptr <Game> game;
	std::shared_ptr <Atis> atis;

	std::shared_ptr <Metar> metar;

	std::shared_ptr <Menu> airports;
	std::shared_ptr <Menu> atis_items;

	int function_key;
	double game_time;
	double previous_fps_time;
	double flash_message_begin;
	double flash_message_time;
	std::string message;
	enum states {MENU, ATIS, GAME, STAT};
	int state;
	void set_flash_message(std::string message);
	
	std::ofstream out;
	
	std::string airport_name;
};

#endif // GAMECONTROLLER_HPP
