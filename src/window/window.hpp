#ifndef _WINDOW_HPP
#define _WINDOW_HPP

#include <iostream>
#include <fstream>
#include <stdexcept>
#include <memory>

#include "../controllers/controller.hpp"
#include "../controllers/game/gamecontroller.hpp"
#include "../models/coordinate/point.hpp"
#include "../tools/settings.hpp"
#include "../tools/database/database.hpp"

/**
	* Base class for window 
	* This provide interface for graphic libraries such as SFML and SDL
	* is should be quite easy to change graphic library.
**/

class Window {
public:
	virtual void init() = 0;
	virtual void run() = 0;
	virtual void load_settings();
protected:
	std::shared_ptr <Settings> settings;
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