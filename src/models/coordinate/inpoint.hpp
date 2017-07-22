#ifndef INPOINT_HPP
#define INPOINT_HPP

#include "navpoint.hpp"

class Inpoint : public Navpoint {
public:
    Inpoint(std::string name, Coordinate p, double spd, double alt, double hdg);
    Inpoint(std::string name, double latitude, double longitude, double spd, double alt, double hdg);
    ~Inpoint();
    double get_altitude();
    double get_speed();
    double get_heading();
protected:
private:
    double speed;
    double altitude;
    double heading;
};

#endif // INPOINT_HPP
