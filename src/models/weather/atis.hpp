#ifndef ATIS_HPP
#define ATIS_HPP

#include "../../tools/settings.hpp"
#include "../../tools/text_tools.hpp"
#include "metar.hpp"
#include "../game_objects/runway.hpp"

#include <vector>
#include <algorithm>

/**
    Atis presenting user inputted data which defines active runways and transition altitude and level.
**/

class Atis {
public:
    Atis(Settings& s, Metar& m);
    ~Atis();
	void load(std::vector <Runway> runways);
    void update();
    void set_departure_runway(std::string dep_rwy);
    void set_landing_runway(std::string lnd_rwy);
    void set_transition_level(int tr_lvl);
    void set_transition_altitude(int tr_alt);

    std::string get_departure_runway();
    std::string get_landing_runway();
    std::list <std::string> get_atis_errors();
    int get_transition_level();
    int get_transition_altitude();
	std::vector <int> get_altitudes();
	std::vector <int> get_levels(int altitude);
	
	bool ok();
	
	bool departure_runway_ok();
	bool landing_runway_ok();
	bool transition_altitude_ok();
	bool transfer_level_ok();
	
	std::map <int, std::vector <int> > get_levels();
protected:
private:
    Settings& settings;
	Metar& metar;
    int transition_level;
    int transition_altitude;
    std::string departure_runway;
    std::string landing_runway;
	std::list <std::string> atis_errors;
	std::vector <Runway> runways;
	std::map <int, std::vector <int> > levels;
	std::vector <int> altitudes;
	
	int calculate_tr_level(int pressure, int altitude);
	
	double calculate_backwind(double wind);
	double calculate_backwind(std::string runway_name);
	bool check_backwind(std::string runway_name);
};


#endif // ATIS_HPP
