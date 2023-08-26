#ifndef _RUNWAY_HPP
#define _RUNWAY_HPP

#include "../coordinate/coordinate.hpp"
#include <tools/tools.hpp>
#include "../../tools/coordinate_tools.hpp"

class Runway {
public:
    Runway(std::string n, Coordinate s, Coordinate e, Coordinate& a);
    Runway();
    ~Runway();

    Coordinate& get_start_place();
    Coordinate& get_end_place();
    Coordinate& get_approach_place();

    double get_heading();
	double get_glidepath();

    std::string get_name();
    bool operator ==(const std::string name);
    bool operator ==(Runway& name);
protected:
private:
    std::string name;
    Coordinate start_place;
    Coordinate end_place;
    Coordinate approach_point;
    double init_heading;
    double turning_altitude;
};

#endif // _RUNWAY_HPP
