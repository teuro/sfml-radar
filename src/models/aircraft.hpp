#ifndef _AIRCRAFT_HPP
#define _AIRCRAFT_HPP

#include <iostream>

#include "inpoint.hpp"
#include "outpoint.hpp"
#include "../tools/tools.hpp"
#include "../tools/settings.hpp"
#include "runway.hpp"
#include "airfield.hpp"
#include "atis.hpp"

/**
	* Aircraft 
	* This representing flying objects. 
**/

class Aircraft {
public:
	/** 
		* Approach constructor
		* @param std::string name
		* @param Settings& settings
		* @param Airfield*& airfield
		* @param Atis*& atis
		* @param Inpoint& in
	**/
	
    Aircraft(std::string name, Settings& settings, Airfield*& airport, Atis*& atis, Inpoint& in);
	
	/** 
		* Departure constructor
		* @param std::string name
		* @param Settings& settings
		* @param Airfield*& airfield
		* @param Atis*& atis
		* @param Outpoint& out
	**/
	
    Aircraft(std::string name, Settings& settings, Airfield*& airport, Atis*& atis, Outpoint& out);
	
    ~Aircraft();
	
	void update(double elapsed);

    Coordinate& get_place();
	
	Navpoint& get_target();

    std::string get_name();

    double get_speed();
	
    double get_clearance_altitude();
    double get_clearance_speed();
    double get_clearance_heading();
	
	double get_altitude();
    double get_peed();
    double get_heading();
	
	bool get_direct();
	bool get_approach();
	bool get_expect();
	
	bool operator ==(std::string t_name) {
		return this->name == t_name;
	}
	
	void set_clearance_speed(double cl_spd);
    void set_clearance_heading(double cl_hdg, int turn = -1);
    void set_clearance_altitude(double cl_alt);
    void set_clearance_approach();
	void cancel_approach();
    void set_clearance_direct(std::string outpoint);

    void set_separation_error(bool t);
    bool get_separation_error();
    void set_place(Coordinate& place);
    void set_approach_runway(std::string name);
	std::string get_landing_runway_name();
	std::string get_target_name();
    int get_type();
	
	void load();
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
    void change_heading();
    void change_speed(double elapsed);
    void change_altitude(double elapsed);
    void change_heading(double elapsed);
    int type;
    bool approach;
    bool direct;
    bool landed;
    bool expect;
    bool final_approach;
	bool check_approach_config();
	void calculate_angle_target(Coordinate& target);
    Settings& settings;
	Navpoint target;
	Coordinate approach_target;
	Runway landing;
	Airfield*& airport;
	Atis*& atis;
};

#endif
