#ifndef _AIRCRAFT_HPP
#define _AIRCRAFT_HPP

#include <iostream>

#include "../coordinate/inpoint.hpp"
#include "../coordinate/outpoint.hpp"
#include <tools/tools.hpp>
#include "../../tools/settings.hpp"
#include "runway.hpp"
#include "airfield.hpp"
#include "../weather/atis.hpp"
#include "Clearances/speed_clearance.hpp"
#include "Clearances/heading_clearance.hpp"
#include "Clearances/altitude_clearance.hpp"
#include "Clearances/expect_clearance.hpp"

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
	
    Aircraft(std::string name, std::shared_ptr <Settings> s, std::shared_ptr <Airfield> airport, std::shared_ptr <Atis> a, Inpoint& in);
	
	/** 
		* Departure constructor
		* @param std::string name
		* @param Settings& settings
		* @param Airfield*& airfield
		* @param Atis*& atis
		* @param Outpoint& out
	**/
	
    Aircraft(std::string name, std::shared_ptr <Settings> s, std::shared_ptr <Airfield> airport, std::shared_ptr <Atis> a, Outpoint& out);
	
    ~Aircraft();
	
	std::string update(double elapsed);

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
	bool get_landing();
	
	bool operator ==(std::string t_name) {
		return this->name == t_name;
	}
	
	void set_clearance(Speed_clearance& cl_spd);
    void set_clearance(Heading_clearance& cl_hdg);
    void set_clearance(Altitude_clearance& cl_alt);
    void set_clearance(Expect_clearance& cl_exp);
	
    std::string set_clearance_approach();
	void cancel_approach();
    void set_clearance_direct(std::string outpoint);
	void set_takeoff_clearance();

    void set_separation_error(bool t);
    bool get_separation_error();
    void set_place(Coordinate& place);
    std::string set_approach_runway(std::string name);
	std::string get_landing_runway_name();
	std::string get_target_name();
    int get_type();
	int get_clearances();
	
	void load();
	bool remove();
	
	double get_in_time();
	double get_out_time();
	int get_points();
	
	void set_in_time(double in);
	void set_out_time(double out);
	void set_points(int points);
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
	
	double v1;
	double vr;
	double v2;
	double vapp;
	double vland;
	double vclimb;
	double vcruise;
	
	double in_time;
	double out_time;
	int points;
	
	double fuel_consumption;
	double fuel_amount;
	double fuel_final_reserve;

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
    bool land;
    bool final_approach;
	bool check_approach_config();
	void calculate_angle_target(Coordinate& target);
	int clearances;
    std::shared_ptr <Settings> settings;
	Navpoint target;
	Coordinate approach_target;
	Runway landing;
	std::shared_ptr <Airfield> airport;
	std::shared_ptr <Atis> atis;
	std::string approach_config_error;
};

#endif
