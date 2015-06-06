#ifndef GAMECONTROLLER_HPP
#define GAMECONTROLLER_HPP

#include <cstdlib>

#include "../models/game.hpp"
#include "../views/game/game_view.hpp"
#include "controller.hpp"

class Gamecontroller : public Controller {
public:
    Gamecontroller(Gameview& gv, Settings& s, Game& g);
    ~Gamecontroller();
    void handle_function_keys(int key);
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
    int function_key;
};


#endif // GAMECONTROLLER_HPP
