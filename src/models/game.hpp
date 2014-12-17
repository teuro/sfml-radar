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

#include "navpoint.hpp"
#include "aircraft.hpp"
#include "airfield.hpp"
#include "../tools/settings.hpp"
#include "clearance.hpp"
#include "database.hpp"

/**
    * Game veryu low-level on MVC model
    * Representing state of game
**/

class Game {
public:
    /**
        * Game main class
        * @param Coordinate& cp center point of game
        * @param Settings& s game settiungs
    **/
	Game(Coordinate& cp, Settings& s);
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
	double get_next_plane() { return this->new_plane; }

    /**
        Loads game needed variables
        * @param void
        * @return void
    **/
	void load();

	void update(double elapsed);

	void select_aircraft(Point& mouse);
	int get_separation_errors() { return this->separation_errors; }
	Point get_place(Point& center, Coordinate& target);
	Aircraft* get_selected() { return this->selected; }

	void set_cl_values(int spd, int alt, int hdg) {
	    this->cl_spd = spd;
	    this->cl_alt = alt;
	    this->cl_hdg = hdg;
    }

    int get_cl_spd() { return this->cl_spd; }
    int get_cl_hdg() { return this->cl_hdg; }
    int get_cl_alt() { return this->cl_alt; }

    void set_cl_spd(int t_spd) { this->cl_spd += t_spd; }
    void set_cl_hdg(int t_hdg) { this->cl_hdg += t_hdg; }
    void set_cl_alt(int t_alt) { this->cl_alt += t_alt; }
    std::vector <Runway>& get_runways();
    TiXmlDocument document;
private:
    void load_airfield(std::string icao);
    void create_plane();
    void build_xml();
	Coordinate& center_point;
	Settings& settings;

	std::vector     <Navpoint>      navpoints;
	std::list       <Aircraft*>     aircrafts;
	std::vector     <Airfield>       airfields;

    Airfield* active_field;
    Aircraft* selected;

    double duration;
    int separation_errors;
    int cl_spd;
    int cl_alt;
    int cl_hdg;
    int new_plane;

    std::list <Aircraft*> :: iterator plane;
};

#endif // _GAME_HPP
