#include "atis.hpp"

Atis::Atis(Settings& s) : settings(s) { 

	this->departure_runway = "04R";
	this->landing_runway = "04L";
	this->transition_altitude = 5000;
	this->transition_level = 55;
	
}

Atis::~Atis() { }

void Atis::update() {
	
}

void Atis::set_departure_runway(std::string dep_rwy) {
    this->departure_runway = dep_rwy;
}

void Atis::set_landing_runway(std::string lnd_rwy) {
    this->landing_runway = lnd_rwy;
}

void Atis::set_transition_altitude(int tr_alt) {
    this->transition_altitude = tr_alt;
}

void Atis::set_transition_level(int tr_lvl) {
    this->transition_level = tr_lvl;
}

std::string Atis::get_departure_runway() {
    return this->departure_runway;
}

std::string Atis::get_landing_runway() {
    return this->landing_runway;
}

int Atis::get_transition_level() {
    return this->transition_level;
}

int Atis::get_transition_altitude() {
    return this->transition_altitude;
}

bool Atis::ok() {
	if (transition_level > 10 && transition_level < 70) {
		if (transition_altitude > 2000 && transition_altitude < 8000) {
			if (departure_runway.length() > 0) {
				if (landing_runway.length() > 0) {
					return true;
				}
			}
		}
	}
	
	return false;
}
