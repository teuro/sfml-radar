#ifndef GAMECONTROLLER_HPP
#define GAMECONTROLLER_HPP

#include <cstdlib>

#include "../models/game.hpp"
#include "../models/atis.hpp"
#include "../models/metar.hpp"
#include "../tools/tools.hpp"
#include "../views/game/game_view.hpp"
#include "../views/atis/atis_view.hpp"
#include "controller.hpp"

struct Clearance {
	double time;
	std::string plane;
	std::string clearance;
};

/**
	* Gamecontroller
	* Responsible for linking gameview and model together
**/

class Gamecontroller : public Controller {
public:
    Gamecontroller(Settings& s, Drawsurface& d);
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
		* @todo This should reflect actual zoom. ATM it's always 7 nm
		* @return void
	**/
    void handle_mouse_release(Point& mouse_start, Point& mouse_end);
    void handle_mouse_wheel(int amount);
    void set_centerpoint(Coordinate& cp);
    Coordinate& get_centerpoint();
    void update(double elapsed);
    void handle_mouse_click(Point& mouse);
    void load();
    void handle_text_input();
    void update_command(std::string command);
    bool is_ok();
    Airfield* get_active_field();
	//std::string get_input();
protected:
private:
	void set_variables();
	void calculate_fps();
	void draw_logic();

	std::list <Clearance> clearances;
	std::list <std::string> quicklist;
	std::list <std::string> matching_elements(std::string input);
    Gameview* gameview;
    Atisview* atisview;
    Game* game;
	Metar* metar;
	Atis* atis;
    int function_key;
	double game_time;
	enum states {PLAYER, ATIS, GAME, STATISTIS};
	int state;
};


#endif // GAMECONTROLLER_HPP
