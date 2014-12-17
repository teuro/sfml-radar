#ifndef GAMECONTROLLER_HPP
#define GAMECONTROLLER_HPP

#include "../models/game.hpp"
#include "../views/game_view.hpp"
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
    void update(double elapsed);
    void handle_mouse_click(Point& mouse);
    void load();
    void handle_text_input(std::string text);
protected:
private:
    Game& game;
    int function_key;
};


#endif // GAMECONTROLLER_HPP
