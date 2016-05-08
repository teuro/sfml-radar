#ifndef ATISCONTROLLER_HPP
#define ATISCONTROLLER_HPP

#include <algorithm>
#include <list>

#include "../models/atis.hpp"
#include "../models/airfield.hpp"
#include "../views/atis/atis_view.hpp"
#include "../tools/tools.hpp"
#include "controller.hpp"
#include "../models/database.hpp"

struct pressure_limit {
    int lower_limit;
    int upper_limit;
    int transfer_altitude;
    int transfer_level;
};

class Atiscontroller : public Controller {
public:
    Atiscontroller(Atisview& av, Settings& s, Atis& a);
    ~Atiscontroller();
    void handle_mouse_click(Point& mouse);
    void handle_mouse_release(Point& mouse_start, Point& mouse_end);
    void handle_mouse_wheel(int amount);
    void update(double elapsed, bool draw);
    std::string handle_function_keys(int key);
    void handle_text_input();
    void update_command(std::string command);
    bool is_ok();
	void load();
protected:
private:
	double calculate_backwind(double wind, double runway);
    Atisview& atisview;
    Atis& atis;
    std::string command;
	std::list <std::string> runways;
	void select_departure(Point& mouse);
	void select_landing(Point& mouse);
};


#endif // ATISCONTROLLER_HPP
