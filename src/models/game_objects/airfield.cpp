#include "airfield.hpp"

Airfield::Airfield(std::string n, Coordinate& p,  int a, int max_speed, int initial_altitude, int acceleration_altitude, int speed_restriction_altitude) : name(n), place(p) {
	this->altitude = a;
	this->max_speed = max_speed;
	this->initial_altitude = initial_altitude;
	this->acceleration_altitude = acceleration_altitude;
	this->speed_restriction_altitude = speed_restriction_altitude;
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

int Airfield::get_max_speed() {
	return this->max_speed;
}

int Airfield::get_initial_altitude() {
	return this->initial_altitude;
}

int Airfield::get_acceleration_altitude() {
	return this->acceleration_altitude;
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
	
	throw std::logic_error("Runway " + name + " not exists");
}

Outpoint& Airfield::get_outpoint(std::string name) {
	for (unsigned int i = 0; i < this->outpoints.size(); ++i) {
		if (this->outpoints[i].get_name() == name) {
			return this->outpoints[i];
		}
	}
	
	throw std::logic_error("Airfield::get_outpoint :: " + name + " not exists");
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

int Airfield::get_speed_restriction_altitude() {
	return this->speed_restriction_altitude;
}
