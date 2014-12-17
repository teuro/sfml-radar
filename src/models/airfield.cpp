#include "airfield.hpp"

Airfield::Airfield(std::string n, Coordinate& p) : name(n), place(p) { }

Airfield::~Airfield() { }

std::vector <Runway>& Airfield::get_runways() {
    return this->runways;
}

Coordinate& Airfield::get_place() {
    return this->place;
}

void Airfield::add_runway(Runway rwy) {
    this->runways.push_back(rwy);
    //std::clog << "Runway " << rwy.get_name() << " added vector size = " << this->runways.size() << std::endl;
}
