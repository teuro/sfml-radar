#ifndef ATIS_HPP
#define ATIS_HPP

#include "../tools/settings.hpp"

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
    Atis(Settings& s);
    ~Atis();
    /**
        * Update updates this based on time from controller.
        * @param double elapsed
        * @return void
    **/
    void update(double elapsed);
    void set_departure_runway(std::string dep_rwy);
    void set_landing_runway(std::string lnd_rwy);
    void set_transition_level(int tr_lvl);
    void set_transition_altitude(int tr_alt);

    std::string get_departure_runway();
    std::string get_landing_runway();
    int get_transition_level();
    int get_transition_altitude();
protected:
private:
    Settings& settings;
    int transition_level;
    int transition_altitude;
    std::string departure_runway;
    std::string landing_runway;
};


#endif // ATIS_HPP
