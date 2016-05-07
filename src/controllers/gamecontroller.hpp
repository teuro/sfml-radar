#ifndef GAMECONTROLLER_HPP
#define GAMECONTROLLER_HPP

#include <cstdlib>

#include "../models/game.hpp"
#include "../models/atis.hpp"
#include "../models/menu.hpp"
#include "../tools/tools.hpp"
#include "../views/game/game_view.hpp"
#include "controller.hpp"

/**
	* Gamecontroller
	* Responsible for linking gameview and model together
**/

class Gamecontroller : public Controller {
public:
    Gamecontroller(Gameview& gv, Settings& s, Game& g, Atis& a);
    ~Gamecontroller();
	/**
		* handle_function_keys
		* reacts any high-level buttons
		* @parameter int key
		* @return void
	**/
    std::string handle_function_keys(int key);
	
	/**
		* handle_mouse_release
		* Moves center point 
		* @parameter Point mouse_start point where mouse left button was pushed
		* @parameter Point mouse_end point where mouse left button was released
		* @return void
	**/
    void handle_mouse_release(Point& mouse_start, Point& mouse_end);
    void handle_mouse_wheel(int amount);
    void set_centerpoint(Coordinate& cp);
    Coordinate& get_centerpoint();
    void update(double elapsed,  bool draw);
    void handle_mouse_click(Point& mouse);
    void load();
    void handle_text_input();
    void update_command(std::string command);
    bool is_ok();
    Airfield* get_active_field();
	//std::string get_input();
protected:
private:
	std::string command;
	std::list <std::string> quicklist;
	std::list <std::string> matching_elements(std::string input);
    Gameview& gameview;
    Game& game;
    Atis& atis;
    int function_key;
};


#endif // GAMECONTROLLER_HPP
