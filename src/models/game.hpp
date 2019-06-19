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

#include "game_objects/aircraft.hpp"
#include "game_objects/airfield.hpp"
#include "weather/atis.hpp"
#include "../tools/settings.hpp"
#include "../tools/tools.hpp"
#include "../tools/text_tools.hpp"
#include "../tools/time_tools.hpp"
#include "../tools/database/database.hpp"

struct Clearance {
	double time;
	std::string plane;
	std::string clearance;
};

struct Game_point {
	double points;
	double in_time;
	double out_time;
	double area_time;
	int clearances;
};

/**
    * Game very low-level on MVC model
    * Representing state of game
**/

typedef std::shared_ptr <Aircraft> aircraft;

class Game {
public:
    /**
        * Game main class
        * @param Settings& s game settiungs
    **/
	
	Game(std::shared_ptr <Settings> settings, std::shared_ptr <Atis> atis);
	~Game();

	/**
        Add's new Navpoint to game
        @param Navpoint& np new nav point
        @return void
	**/
	void add_point(Navpoint np);

	std::list <aircraft> get_aircrafts();
	
	aircraft selected;

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
        * @param std::string
        * @return void
    **/
	
	void load(std::string airfield);

    /**
        * Calculate new state of game
        * @param double elapsed
        * @return void
    **/
	
	void update(double elapsed);

	/**
		* Returns selected plane
		* @param void
		* return Aircraft*
	**/
	
	aircraft get_selected();
	
	/**
		* Returns selected airfield
		* @param void
		* return Airfield*
	**/
	
	std::shared_ptr <Airfield> get_active_field();
	
	std::shared_ptr <Atis> get_atis();
	
	void create_plane();
	/** 
		* build clearance
		* spilts command into peaces and try to make proper clearance
		* @todo fix turn there should be left or right every else should be error
		* @param command
		* @return void
		
	**/
	void build_clearance(std::string command);
	
	void create_planes(int amount);
	
	int get_handled_planes();
	int get_planes_count();
	int get_separation_errors();
	int get_new_plane();
	std::map <std::string, Game_point> get_points();
	double get_game_points();
	std::list <Clearance> get_clearances();
	bool ok();
	std::string get_clearance_error();
	std::string get_game_error();
	void remove_first_game_error();
	
	std::string get_message();
	void clear_errors();
	int get_level();
	void set_level(int level);
	std::list <aircraft> get_holdings();
private:
	int level;
    void load_airfield(std::string icao);
    void check_collision();
    void handle_holdings();
	void load_navpoints(int airfield_id);
	void load_runways(int airfield_id);
	Inpoint select_inpoint();
	Outpoint select_outpoint();
    bool is_free(Inpoint& navpoint);
	void calculate_points(int type, double clearance_count, std::string plane);
	int calculate_clearances(std::string name);
	bool check_aircrafts(std::string name);
	std::shared_ptr <Settings> settings;

	std::vector     <Navpoint>					navpoints;
	std::list       <aircraft>					aircrafts;
	std::list		<aircraft>					holdings;
	std::queue      <std::string>				display_messages;
	std::queue      <std::string>				game_errors;
	std::list       <aircraft>					errors;
	std::list       <Clearance>					clearances;
	std::map		<std::string, Game_point>	points;
	
	std::string command;

    std::shared_ptr <Airfield> active_field;
	std::shared_ptr <Atis> atis;
	std::unique_ptr <Menu> menu;
    double duration;
	double pop_holdings;
    int separation_errors;
    int new_plane;
	unsigned int handled_planes;
	bool loaded;
};

#endif // _GAME_HPP
