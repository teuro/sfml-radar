#ifndef ATIS_HPP
#define ATIS_HPP

#include "../../tools/settings.hpp"
#include "../../tools/text_tools.hpp"
#include "metar.hpp"
#include "../game_objects/runway.hpp"
#include "../game_objects/airfield.hpp"
#include "../menu/menu.hpp"
#include "../../views/menu/menu_view.hpp"

#include <vector>
#include <algorithm>

/**
    Atis presenting user inputted data which defines active runways and transition altitude and level.
**/

class Atis {
public:
    Atis(std::shared_ptr <Settings> s, Metar& m);
    ~Atis();
	void load();
    void update(int amount);
	
	int get_transition_level();
    int get_transition_altitude();
	std::vector <int> get_altitudes();
	std::vector <int> get_levels(int altitude);
	std::vector <Runway> get_runways();

    std::list <std::string> get_atis_errors();
	
	Runway& get_departure_runway();
	Runway& get_landing_runway();
	
	Menu* get_menu();
	
	bool ok();
	
	bool departure_runway_ok();
	bool landing_runway_ok();
	bool transition_altitude_ok();
	bool transfer_level_ok();
	
	std::map <int, std::vector <int> > get_levels();
	
	void set_value(std::string value);
	
	void set_airfield(std::shared_ptr <Airfield> airfield);
protected:
private:
    std::shared_ptr <Settings> settings;
	Metar& metar;
	std::shared_ptr <Airfield> active_field;
    int transition_level;
    int transition_altitude;
    Runway departure_runway;
    Runway landing_runway;
	std::list <std::string> atis_errors;
	std::vector <Runway> runways;
	std::map <int, std::vector <int> > levels;
	std::vector <int> altitudes;
	
	int calculate_tr_level(int pressure, int altitude);
	
	double calculate_backwind(double wind);
	double calculate_backwind(Runway& runway_name);
	bool check_backwind(Runway& runway_name);
	
	void set_departure_runway(std::string dep_rwy);
    void set_landing_runway(std::string lnd_rwy);
    void set_transition_level(int tr_lvl);
    void set_transition_altitude(int tr_alt);
	
	enum state_names {DEPARTURE, LANDING, ALTITUDE, LEVEL };
	int state;
	Menu* menu;
};


#endif // ATIS_HPP
