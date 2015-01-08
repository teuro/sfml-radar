#ifndef _CLEARANCE_HPP
#define _CLEARANCE_HPP

#include <string>

class Clearance {
public:
    Clearance(int spd, int alt, int hdg, int trn);
    ~Clearance();
    Clearance();

    int get_speed();
    int get_altitude();
    int get_heading();
    int get_turn();
private:
    int speed;
    int altitude;
    int heading;
    int turn;
};

#endif // _CLEARANCE_HPP
