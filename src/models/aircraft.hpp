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

class Aircraft {
public:
    Aircraft(std::string name, Settings& settings, Airfield*& airport, Atis*& atis, Inpoint& in);
    Aircraft(std::string name, Settings& settings, Airfield*& airport, Atis*& atis, Outpoint& out);
	
    ~Aircraft();
	
	void update(double elapsed);

    Coordinate& get_place();
	
	Navpoint& get_target();

    std::string get_name();

    double get_speed();
	
    double get_heading();
	
    double get_altitude();
    double get_clearance_speed();
    double get_clearance_heading();
	
	bool operator ==(std::string t_name) {
		return this->name == t_name;
	}
	
    double get_clearance_altitude();
	
	void set_clearance_speed(double cl_spd);
    void set_clearance_heading(double cl_hdg, int turn = -1);
    void set_clearance_altitude(double cl_alt);
    void set_clearance_approach();
	void cancel_approach();
    void set_clearance_direct();

    void set_separation_error(bool t);
    bool get_separation_error();
    void set_place(Coordinate& place);
    void set_approach_runway(std::string name);
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
	Runway landing;
	Airfield*& airport;
	Atis*& atis;
};

#endif
