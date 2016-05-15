#ifndef PLAYERCONTROLLER_HPP
#define PLAYERCONTROLLER_HPP

#include "../models/player.hpp"
#include "../views/player/player_view.hpp"
#include "controller.hpp"

class Playercontroller : public Controller {
public:
    Playercontroller(Drawsurface& d, Settings& s);
    ~Playercontroller();
    void handle_mouse_click(Point& mouse);
    void handle_mouse_release(Point& mouse_start, Point& mouse_end);
    void handle_mouse_wheel(int amount);
    void update(double elapsed, bool draw) { }
    std::string handle_function_keys(int key);
    void handle_text_input();
    void update_command(std::string command);
    bool is_ok();
	void load();
protected:
private:
    Playerview* playerview;
    Player* player;
};


#endif // PLAYERCONTROLLER_HPP
