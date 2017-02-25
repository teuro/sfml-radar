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

#include "aircraft.hpp"
#include "airfield.hpp"
#include "atis.hpp"
#include "../tools/settings.hpp"
#include "../tools/tools.hpp"
#include "../tools/database/database.hpp"

struct Clearance {
	double time;
	std::string plane;
	std::string clearance;
};

struct Game_point {
	std::string plane;
	double points;
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
	Game(Settings& s, Atis*& atis);
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
	std::list <Game_point> get_points();
	double get_game_points();
	std::list <Clearance> get_clearances();
	bool ok();
	std::string get_clearance_error();
	void remove_first_error();
private:
    void load_airfield(std::string icao);
    void check_collision();
    void handle_holdings();
	void load_navpoints(std::map <std::string, std::string> variables);
	void load_runways(std::map <std::string, std::string> variables);
	Inpoint select_inpoint();
    bool is_free(Inpoint& navpoint);
	void calculate_points(int type, double clearance_count, std::string plane);
	int calculate_clearances(std::string name);
	bool check_aircrafts(std::string name);
	Coordinate centerpoint;
	Settings& settings;

	std::vector     <Navpoint>		navpoints;
	std::list       <Aircraft*>		aircrafts;
	std::queue      <Aircraft*>		holdings;
	std::queue      <std::string>	clearance_errors;
	std::list       <Aircraft*>		errors;
	std::list       <Clearance>		clearances;
	std::list       <Game_point>	points;
	std::vector     <Airfield>		airfields;

	std::string command;

    Airfield* active_field;
    Runway departure;
    Runway landing;
	Atis*& atis;

    double duration;
    int separation_errors;
    int new_plane;
	unsigned int handled_planes;
};

#endif // _GAME_HPP
