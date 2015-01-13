#ifndef _RUNWAY_HPP
#define _RUNWAY_HPP

#include "coordinate.hpp"
#include "../tools/tools.hpp"

class Runway {
public:
    Runway(std::string n, Coordinate s, Coordinate e);
    ~Runway();

    Coordinate& get_start_place();
    Coordinate& get_end_place();
    double get_length();
    double get_heading();
    std::string get_name();
    bool operator ==(const std::string name);
protected:
private:
    std::string name;
    Coordinate start_place;
    Coordinate end_place;
    double init_heading;
    double turning_altirude;
};


#endif // _RUNWAY_HPP
