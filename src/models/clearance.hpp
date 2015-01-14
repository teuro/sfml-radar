#ifndef _CLEARANCE_HPP
#define _CLEARANCE_HPP

#include "runway.hpp"

#include <string>

class Clearance {
public:
    Clearance(int spd, int alt, int hdg, int trn);
    Clearance(Runway*& lnd);
    Clearance();
    ~Clearance();

    int get_speed();
    int get_altitude();
    int get_heading();
    int get_turn();
    Runway*& get_landing();
private:
    int speed;
    int altitude;
    int heading;
    int turn;
    Runway* landing;
};

#endif // _CLEARANCE_HPP
