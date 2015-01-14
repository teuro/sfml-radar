#ifndef _GAME_HPP
#define _GAME_HPP

#include <iostream>
#include <vector>
#include <list>
#include <map>
#include <fstream>
#include <stdexcept>
#include <dirent.h>
#include <tinyxml/tinyxml.h>
#include <tinyxpath/xpath_processor.h>
#include <queue>

#include "navpoint.hpp"
#include "aircraft.hpp"
#include "airfield.hpp"
#include "../tools/settings.hpp"
#include "clearance.hpp"
#include "database.hpp"
#include "metar.hpp"
#include "atis.hpp"

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
	Game(Coordinate& cp, Settings& s, Atis& a);
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
	void load();

	void update(double elapsed);

	void select_aircraft(Point& mouse);
	void select_aircraft(std::string callsign);
	int get_separation_errors();
	Point get_place(Point& center, Coordinate& target);
	Aircraft* get_selected();
	Airfield* get_active_field();

	void set_clearance(std::string callsign, std::vector <std::string> command);

    TiXmlDocument document;

    Metar& get_metar();

    void set_active_runways(Runway* dep, Runway* lnd);

    void set_command(std::string command);
    std::string get_command();
private:
    void load_airfield(std::string icao);
    /**
        * Create new Aircraft* element
        * @param none
        * @param none
    **/
    void create_plane();
    void build_xml();
	Coordinate& center_point;
	Settings& settings;
	Atis& atis;

	std::vector     <Navpoint>      navpoints;
	std::list       <Aircraft*>     aircrafts;
	std::queue      <Aircraft*>     holdings;
	std::vector     <Airfield>      airfields;

	std::string command;

    Airfield* active_field;
    Aircraft* selected;
    Runway* departure;
    Runway* landing;

    double duration;
    int separation_errors;
    int new_plane;

    Metar metar;
};

#endif // _GAME_HPP
