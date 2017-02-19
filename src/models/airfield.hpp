#ifndef _AIRFIELD_HPP
#define _AIRFIELD_HPP

#include <vector>

#include "runway.hpp"
#include "inpoint.hpp"
#include "outpoint.hpp"
#include "../tools/tools.hpp"

class Airfield {
public:
    Airfield(std::string n, Coordinate& p);
    ~Airfield();
    std::vector <Runway>& get_runways();
    Coordinate& get_place();
    void add_runway(Runway rwy);
	Runway& get_runway(std::string name);
	
	Inpoint& get_inpoint(std::string name);
	Outpoint& get_outpoint(std::string name);
	
	Inpoint& get_random_inpoint();
	Outpoint& get_random_outpoint();
	
	std::vector <Navpoint>& get_navpoints();
    int get_altitude();
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
	std::vector <Outpoint> outpoints;
	std::vector <Inpoint> inpoints;
	std::vector <Navpoint> navpoints;
};


#endif // _AIRFIELD_HPP
