#ifndef _GAME_HPP
#define _GAME_HPP

#include <iostream>
#include <vector>
#include <list>
#include <map>
#include <fstream>
#include <stdexcept>
#include <queue>
#include <algorithm>

#include "inpoint.hpp"
#include "outpoint.hpp"
#include "aircraft.hpp"
#include "airfield.hpp"
#include "../tools/settings.hpp"
#include "../tools/tools.hpp"
#include "../tools/database/database.hpp"

/**
    * Game very low-level on MVC model
    * Representing state of game
**/

class Game {
public:
    /**
        * Game main class
        * @param Coordinate& cp center point of game
        * @param Settings& s game settiungs
    **/
	Game(Settings& s);
	~Game();

    /**
        * Get center point of game
        * @param none
        * @return Coordinate& center point coordinates
    **/
	Coordinate& get_centerpoint();

	/**
        * Set new center poitn to the game
        * @param Coordinate& cp new center point
        * @return void
	**/
	void set_centerpoint(Coordinate& cp);

	/**
        Add's new Navpoint to game
        @param Navpoint& np new nav point
        @return void
	**/
	void add_point(Navpoint np);

	std::vector <Navpoint>& get_navpoints();
	std::list <Aircraft*> get_aircrafts();
	std::vector <Airfield>& get_airfields();

    /**
        Get's game duration pure value
        * @param void
        * @return double
    **/
	double get_duration();

	/**
        Return time when new plane is created
        * @param void
        * @return double
	**/
	double get_next_plane();

    /**
        Loads game needed variables
        * @param void
        * @return void
    **/
	void load(std::string airfield, std::string dep, std::string lnd);

    /**
        Calculate new state of game
        * @param double elapsed
        * @return void
    **/
	void update(double elapsed);

    /**
        * Select a plane
        * @param Point& mouse
        * @return void
    **/
	void select_aircraft(Point& mouse);

	/**
        * Select a plane
        * @param std::string callsign
        * @return void
	**/
	void select_aircraft(std::string callsign);

	Aircraft* get_selected();
	Airfield* get_active_field();
	
	void create_plane();
	std::string get_metar();
	void build_clearance(std::string command);
private:
    void load_airfield(std::string icao);
    void check_collision();
    void handle_holdings();
    bool is_free(Inpoint& navpoint);
	Coordinate centerpoint;
	Settings& settings;

	std::vector     <Outpoint>		outpoints;
	std::vector     <Inpoint>		inpoints;
	std::vector     <Navpoint>		navpoints;
	std::list       <Aircraft*>		aircrafts;
	std::queue      <Aircraft*>		holdings;
	std::list       <Aircraft*>		errors;
	std::vector     <Airfield>		airfields;

	std::string command;

    Airfield* active_field;
    Aircraft* selected;
    Runway departure;
    Runway landing;

    double duration;
    int separation_errors;
    int new_plane;
	
	enum turn {LEFT = -1, RIGHT = 1};
};

#endif // _GAME_HPP
