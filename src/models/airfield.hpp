#ifndef _AIRFIELD_HPP
#define _AIRFIELD_HPP

#include <vector>

#include "runway.hpp"

class Airfield {
public:
    Airfield(std::string n, Coordinate& p);
    ~Airfield();
    std::vector <Runway>& get_runways();
    Coordinate& get_place();
    void add_runway(Runway rwy);
    int get_altitude();
protected:
private:
    std::vector <Runway> runways;
    std::string name;
    Coordinate& place;
    int altitude;
};


#endif // _AIRFIELD_HPP
