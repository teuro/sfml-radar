#include "outpoint.hpp"

Outpoint::Outpoint(std::string name, Coordinate p) : Navpoint(name, p) {

}

Outpoint::Outpoint(std::string name, double latitude, double longitude) : Navpoint(name, latitude, longitude) {

}

Outpoint::~Outpoint() {

}
