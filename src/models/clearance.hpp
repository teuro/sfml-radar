#ifndef _CLEARANCE_HPP
#define _CLEARANCE_HPP

class Clearance {
public:
    Clearance(std::string cs, double t, int spd, int alt, int hdg, int trn) : callsing(cs), time(t), speed(spd), altitude(alt), heading(hdg), turn(trn) { }
    ~Clearance() { }
    Clearance() { }

    int get_speed() { return this->speed; }
    int get_altitude() { return this->altitude; }
    int get_heading() { return this->heading; }
    int get_turn() { return this->turn; }
private:
    std::string callsing;
    double time;
    int speed;
    int altitude;
    int heading;
    int turn;
};


#endif // _CLEARANCE_HPP
