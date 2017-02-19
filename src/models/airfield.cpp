#include "airfield.hpp"

Airfield::Airfield(std::string n, Coordinate& p) : name(n), place(p) { 
	this->altitude = 150;
}

Airfield::~Airfield() { }

std::vector <Runway>& Airfield::get_runways() {
	return this->runways;
}

Coordinate& Airfield::get_place() {
    return this->place;
}

void Airfield::add_runway(Runway rwy) {
    this->runways.push_back(rwy);
}

int Airfield::get_altitude() {
    return this->altitude;
}

std::string Airfield::get_name() {
    return this->name;
}


Runway& Airfield::get_runway(std::string name) {
	for (unsigned int i = 0; i < this->runways.size(); ++i) {
		if (this->runways[i].get_name() == name) {
			return this->runways[i];
		}
	}
	
	throw std::logic_error("Airfield::get_runway :: " + name + " not exists");
}
