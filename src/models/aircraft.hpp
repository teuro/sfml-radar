#ifndef _AIRCRAFT_HPP
#define _AIRCRAFT_HPP

#include "navpoint.hpp"
#include "../tools/tools.hpp"
#include "../tools/settings.hpp"
#include "runway.hpp"

class Aircraft {
public:
    Aircraft(std::string name, double speed, double heading, double altitude, Coordinate p, int type, Settings& s, Runway& land);
    ~Aircraft();
    void update(double elapsed);

    Coordinate& get_place();
    std::string get_name();

    double get_speed();
    double get_heading();
    double get_altitude();
    int get_turn();

    double get_clearance_speed();
    double get_clearance_heading();
    double get_clearance_altitude();
	
	void set_clearance_speed(double cl_spd);
    void set_clearance_heading(double cl_hdg, int turn = -1);
    void set_clearance_altitude(double cl_alt);
    void set_clearance_approach();

    void set_separation_error(bool t);
    void set_target(Navpoint* target);
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
    Navpoint* target;
    bool approach;
    bool landed;
    Settings& settings;
	Runway& landing;
};


#endif
