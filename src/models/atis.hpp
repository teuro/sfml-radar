#ifndef ATIS_HPP
#define ATIS_HPP

#include "../tools/settings.hpp"
#include "metar.hpp"

/**
    Atis presenting user inputted data which defines active runways and transition altitude and level.
**/

class Atis {
public:
    /**
        * Constructor
        * @param Settings
        * @return none
    **/
    Atis(Settings& s, Metar& m);
    ~Atis();
    /**
        * Update updates this based on time from controller.
        * @param double elapsed
        * @return void
    **/
    void update();
    void set_departure_runway(std::string dep_rwy);
    void set_landing_runway(std::string lnd_rwy);
    void set_transition_level(int tr_lvl);
    void set_transition_altitude(int tr_alt);

    std::string get_departure_runway();
    std::string get_landing_runway();
    int get_transition_level();
    int get_transition_altitude();
	
	bool ok();
protected:
private:
    Settings& settings;
	Metar& metar;
    int transition_level;
    int transition_altitude;
    std::string departure_runway;
    std::string landing_runway;
	
	int calculate_tr_level(int pressure, int altitude);
};


#endif // ATIS_HPP
