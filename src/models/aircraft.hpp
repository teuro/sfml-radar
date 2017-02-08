#ifndef _AIRCRAFT_HPP
#define _AIRCRAFT_HPP

#include <iostream>

#include "navpoint.hpp"
#include "../tools/tools.hpp"
#include "../tools/settings.hpp"
#include "runway.hpp"

class Aircraft {
public:
    Aircraft(std::string name, double speed, double heading, double altitude, Coordinate p, int type, Settings& s, Runway& land, Navpoint& out);
    ~Aircraft();
    
	/**
		* update
		* Hndles aircrafts movement. 
		* @param double elapsed time since previous update in milliseconds
		* @return void
	**/
	
	void update(double elapsed);
	
	/**
		* get_place
		* returns aircraft's current position in map coordinates 
		* @param void
		* @return Coordinate&
	**/
	
    Coordinate& get_place();
	
	/**
		* get_target
		* returns aircraft's current target point.
		* @param void
		* @return Navpoint&
	**/
	
	Navpoint& get_target();
	
	/**
		* get_name
		* returns aircraft's name (callsign).
		* @param void
		* @return std::string
	**/
	
    std::string get_name();
	
	/**
		* get_speed
		* returns aircraft's current speed in knots.
		* @param void
		* @return double
	**/
	
    double get_speed();
	
	/**
		* get_heading
		* returns aircraft's current heading in radians.
		* @param void
		* @return double
	**/
	
    double get_heading();
	
	/**
		* get_altitude
		* returns aircraft's current altitude in feets.
		* @param void
		* @return double
	**/
	
    double get_altitude();
    
	/**
		* get_clearance_speed
		* returns aircraft's current get_clearance_speed in knots.
		* @param void
		* @return double
	**/
	
    double get_clearance_speed();
	
	/**
		* get_clearance_heading
		* returns aircraft's current get_clearance_heading in radians.
		* @param void
		* @return double
	**/
	
    double get_clearance_heading();
	
	/**
		* get_clearance_altitude
		* returns aircraft's current get_clearance_altitude in feets.
		* @param void
		* @return double
	**/
	
    double get_clearance_altitude();
	
	void set_clearance_speed(double cl_spd);
    void set_clearance_heading(double cl_hdg, int turn = -1);
    void set_clearance_altitude(double cl_alt);
    void set_clearance_approach();
    void set_clearance_direct();

    void set_separation_error(bool t);
    bool get_separation_error();
    void set_place(Coordinate& place);
    int get_type();
protected:
private:
    Coordinate place;
    std::string name;
    bool separation_error;

    double speed;
    double heading;
    double altitude;

    double clearance_speed;
    double clearance_altitude;
    double clearance_heading;
    int turn;

    void change_speed();
    void change_altitude(double elapsed);
    void change_heading();
    double change_parameter(double elapsed, double value, double _value, double change, int turn = 0);
    int type;
    bool approach;
    bool direct;
    bool landed;
	bool check_approach_config();
	void calculate_angle_target(Coordinate& target);
    Settings& settings;
	Navpoint target;
	Runway& landing;
};


#endif
