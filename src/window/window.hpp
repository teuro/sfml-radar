#ifndef _WINDOW_HPP
#define _WINDOW_HPP

#include <iostream>
#include <fstream>
#include <stdexcept>

#include <sqlite3pp.h>

#include "../controllers/controller.hpp"
#include "../controllers/gamecontroller.hpp"
#include "../models/coordinate/point.hpp"
#include "../tools/settings.hpp"
#include "../tools/database/database.hpp"

class Window {
public:
	virtual void init() = 0;
	virtual void close() = 0;
	virtual void run() = 0;
	virtual void load_settings() = 0;
	virtual void handle_events() = 0;
protected:
	Settings settings;
	enum types {PLAYER, ATIS, GAME};
	Point mouse_start;
    Point mouse_end;
    Point mouse;

    std::string input_string;
    bool draw;
    int counter;
    int input_action;
	int game_state;
	double time; 
};

#endif