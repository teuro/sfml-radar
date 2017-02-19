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

Inpoint& Airfield::get_inpoint(std::string name) {
	for (unsigned int i = 0; i < this->inpoints.size(); ++i) {
		if (this->inpoints[i].get_name() == name) {
			return this->inpoints[i];
		}
	}
	
	throw std::logic_error("Airfield::get_inpoint :: " + name + " not exists");
}

Outpoint& Airfield::get_outpoint(std::string name) {
	for (unsigned int i = 0; i < this->outpoints.size(); ++i) {
		if (this->outpoints[i].get_name() == name) {
			return this->outpoints[i];
		}
	}
	
	throw std::logic_error("Airfield::get_outpoint :: " + name + " not exists");
}

Inpoint& Airfield::get_random_inpoint() {
	int place = Tools::rnd(0, (int)this->inpoints.size()-1);
	return this->inpoints[place];
}

Outpoint& Airfield::get_random_outpoint() {
	int place = Tools::rnd(0, (int)this->outpoints.size()-1);
	return this->outpoints[place];
}

std::vector <Outpoint>& Airfield::get_outpoints() {
	return this->outpoints;
}

std::vector <Navpoint>& Airfield::get_navpoints() {
	return this->navpoints;
}

std::vector <Inpoint>& Airfield::get_inpoints() {
	return this->inpoints;
}

void Airfield::add_inpoint(Inpoint ip) {
	this->inpoints.push_back(ip);
	this->navpoints.push_back(ip);
}

void Airfield::add_outpoint(Outpoint op) {
	this->outpoints.push_back(op);
	this->navpoints.push_back(op);
}
