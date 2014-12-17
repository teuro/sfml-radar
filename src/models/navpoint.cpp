#include "navpoint.hpp"

Navpoint::Navpoint(std::string name, Coordinate p) : place(p) {
    this->name = name;
}

Navpoint::Navpoint(std::string name, double latitude, double longitude) {
    this->name = name;
    Coordinate tmp(latitude, longitude);
    this->place = tmp;
}

Navpoint::~Navpoint() { }

Coordinate& Navpoint::get_place() {
    return this->place;
}

std::string Navpoint::get_name() {
    return this->name;
}
