#ifndef NAVPOINT_HPP
#define NAVPOINT_HPP

#include <iostream>

#include "coordinate.hpp"

class Navpoint {
public:
    Navpoint(std::string name, Coordinate p);
    Navpoint();
    Navpoint(std::string name, double latitude, double longitude);
    ~Navpoint();
    Coordinate& get_place();
    std::string get_name();
    bool operator==(std::string name);
protected:
    Coordinate place;
    std::string name;
};

#endif // NAV_POINT_HPP
