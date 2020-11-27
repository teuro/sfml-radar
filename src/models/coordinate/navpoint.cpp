#include "navpoint.hpp"

Navpoint::Navpoint() { }

Navpoint::Navpoint(std::string n, Coordinate p, std::string t) : name(n), place(p), type(t) { }

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

bool Navpoint::operator ==(std::string name) {
    return this->name == name;
}
std::string Navpoint::get_type() {
	return this->type;
}
