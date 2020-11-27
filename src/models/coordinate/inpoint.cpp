#include "inpoint.hpp"

Inpoint::Inpoint(std::string name, Coordinate p, double spd, double alt, double hdg) : Navpoint(name, p, "inpoint"), speed(spd), altitude(alt), heading(hdg) { }

Inpoint::~Inpoint() {}

double Inpoint::get_altitude() {
    return this->altitude;
}

double Inpoint::get_speed() {
    return this->speed;
}

double Inpoint::get_heading() {
    return this->heading;
}
