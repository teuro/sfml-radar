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

struct Clearance {
	double time;
	std::string plane;
	std::string clearance;
};

/**
    * Game very low-level on MVC model
    * Representing state of game
**/

class Game {
public:
    /**
        * Game main class
        * @param Settings& s game settiungs
    **/
	Game(Settings& s);
	~Game();

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
	
	Aircraft* selected;

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
	void load(std::string airfield);

    /**
        * Calculate new state of game
        * @param double elapsed
        * @return void
    **/
	void update(double elapsed);
	
	void set_runways(std::string t_departure, std::string t_landing);
	
	/**
        * Select a plane
        * @param std::string callsign
        * @return void
	**/
	void select_aircraft(std::string callsign);

	Aircraft* get_selected();
	Airfield* get_active_field();
	
	void create_plane();
	void build_clearance(std::string command);
	
	int get_handled_planes();
	int get_planes_count();
	int get_separation_errors();
	int get_new_plane();
	int get_points();
	std::list <Clearance> get_clearances();
private:
    void load_airfield(std::string icao);
    void check_collision();
    void handle_holdings();
    bool is_free(Inpoint& navpoint);
	void calculate_points(int type, int clearance_count);
	int calculate_clearances(std::string name);
	Coordinate centerpoint;
	Settings& settings;

	std::vector     <Outpoint>		outpoints;
	std::vector     <Inpoint>		inpoints;
	std::vector     <Navpoint>		navpoints;
	std::list       <Aircraft*>		aircrafts;
	std::queue      <Aircraft*>		holdings;
	std::list       <Aircraft*>		errors;
	std::list       <int>			points;
	std::list       <Clearance>		clearances;
	std::vector     <Airfield>		airfields;

	std::string command;

    Airfield* active_field;
    Runway departure;
    Runway landing;

    double duration;
    int separation_errors;
    int new_plane;
	int handled_planes;
	
	enum turn {LEFT = -1, RIGHT = 1};
	enum types {DEPARTURE, APPROACH};
	Queryresult airlines;
};

#endif // _GAME_HPP
