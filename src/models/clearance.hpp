#ifndef _CLEARANCE_HPP
#define _CLEARANCE_HPP

#include <string>

class Clearance {
public:
    Clearance(int spd, int alt, int hdg, int trn, bool approach);
    Clearance();
    ~Clearance();

    int get_speed();
    int get_altitude();
    int get_heading();
    int get_turn();
	bool get_approach();
private:
    int speed;
    int altitude;
    int heading;
    int turn;
	bool approach;
};

#endif // _CLEARANCE_HPP
