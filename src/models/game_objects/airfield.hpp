#ifndef _AIRFIELD_HPP
#define _AIRFIELD_HPP

#include <vector>

#include "runway.hpp"
#include "../coordinate/inpoint.hpp"
#include "../coordinate/outpoint.hpp"
#include "../../tools/tools.hpp"

class Airfield {
public:
    Airfield(std::string n, Coordinate& p, int max_speed, int initial_altitude, int acceleration_altitude, int speed_restriction_altitude);
    ~Airfield();
    std::vector <Runway>& get_runways();
    Coordinate& get_place();
    void add_runway(Runway rwy);
	Runway& get_runway(std::string name);
	
	Outpoint& get_outpoint(std::string name);
	
	std::vector <Navpoint>& get_navpoints();
    int get_altitude();
	int get_max_speed();
	int get_initial_altitude();
	int get_acceleration_altitude();
	int get_speed_restriction_altitude();
	std::string get_name();
	void add_inpoint(Inpoint ip);
	void add_outpoint(Outpoint op);
	
	std::vector <Outpoint>& get_outpoints(); 
	std::vector <Inpoint>& get_inpoints(); 
protected:
private:
    std::vector <Runway> runways;
    std::string name;
    Coordinate& place;
    int altitude;
	int initial_altitude;
	int max_speed;
	int acceleration_altitude;
	int speed_restriction_altitude;
	std::vector <Outpoint> outpoints;
	std::vector <Inpoint> inpoints;
	std::vector <Navpoint> navpoints;
};

#endif // _AIRFIELD_HPP
