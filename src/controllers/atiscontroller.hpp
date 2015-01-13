#ifndef ATISCONTROLLER_HPP
#define ATISCONTROLLER_HPP

#include <algorithm>

#include "../models/atis.hpp"
#include "../models/game.hpp"
#include "../models/airfield.hpp"
#include "../views/atis/atis_view.hpp"
#include "../tools/tools.hpp"
#include "controller.hpp"

struct pressure_limit {
    int lower_limit;
    int upper_limit;
    int transfer_altitude;
    int transfer_level;
};

class Atiscontroller : public Controller {
public:
    Atiscontroller(Atisview& av, Settings& s, Atis& a, Game& g);
    ~Atiscontroller();
    void handle_mouse_click(Point& mouse);
    void handle_mouse_release(Point& mouse_start, Point& mouse_end);
    void handle_mouse_wheel(int amount);
    void update(double elapsed, bool draw);
    void handle_function_keys(int key);
    void handle_text_input();
    void update_command(std::string command);
    bool is_ok();
protected:
private:
    Atisview& atisview;
    Atis& atis;
    Game& game;
    std::string input;
};


#endif // ATISCONTROLLER_HPP
