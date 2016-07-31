#ifndef _PROGRAM_HPP
#define _PROGRAM_HPP

#include <iostream>
#include <fstream>
#include <stdexcept>

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <sqlite3pp.h>

#include "controllers/gamecontroller.hpp"
#include "models/point.hpp"
#include "tools/settings.hpp"
#include "tools/database/database.hpp"

/**
 * @file
 * @author  Juha Teurokoski <teurokoski.juha@gmail.com>
 * @version 1.0
 *
 * @section LICENSE
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details at
 * http://www.gnu.org/copyleft/gpl.html
 *
 * @section DESCRIPTION
 *
 * The Program class is highest level on game. It's not know anything of game
 * You may use it with other games. Basically program just reads events and
 * sends them to proper controllers.
 *
 * On run method controls the game
 * 1) reads events
 * 2) handles events
 * 3) calculating time
 * 4) update game via gamecontroller
 * 5) set up FPS and draw
 */
class Program {
public:
    Program();
    ~Program();

    /**
        * Run main method runs until receive quit event
        * @param none
        * @return void
        *
        *
    **/
    void run();

    /**
        * init
        * @param none
        * @return none
        *
        * Initialize everything
        * Loads settings from database
    **/
    void init();

    /**
        * close
        * @param none
        * @return none
        *
        * Do nothing
        * @todo Close open items
    **/
    void close();
	
	
    unsigned int screen_width;
    unsigned int screen_height;
    std::string program_name;
	Settings settings;
protected:
private:
    sf::Clock clock;
	sf::Time time_change;

    bool handle_event(sf::Event& event, Controller& ctrl, sf::RenderWindow& window);

    enum types {PLAYER, ATIS, GAME};

    int game_state;

    void load();

    Point mouse_start;
    Point mouse_end;

    sqlite3* database;
    bool draw;
    int counter;
    std::string input_string;
    int input_action;
	
	bool is_drawable();
	bool handle_events(Controller& ctrl, sf::RenderWindow& window);
};


#endif // _PROGRAM_HPP
