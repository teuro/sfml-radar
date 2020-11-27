#ifndef NAVPOINT_HPP
#define NAVPOINT_HPP

#include <iostream>

#include "coordinate.hpp"

class Navpoint {
public:
    Navpoint(std::string name, Coordinate place, std::string type);
    Navpoint();
    Navpoint(std::string name, double latitude, double longitude);
    ~Navpoint();
    Coordinate& get_place();
    std::string get_name();
    bool operator==(std::string name);
protected:
    std::string name;
    Coordinate place;
	std::string type;
public:
	virtual std::string get_type();
};

#endif // NAV_POINT_HPP
