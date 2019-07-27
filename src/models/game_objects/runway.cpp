#include "runway.hpp"

Runway::Runway(std::string n, Coordinate s, Coordinate e, Coordinate& a) : name(n), start_place(s), end_place(e), approach_point(a) { 
	this->init_heading = -1;
	this->turning_altitude = -1;
}

Runway::Runway() {}

Runway::~Runway() { }

Coordinate& Runway::get_start_place() {
    return this->start_place;
}

Coordinate& Runway::get_end_place() {
    return this->end_place;
}

Coordinate& Runway::get_approach_place() {
	return this->approach_point;
}

double Runway::get_heading() {
	double hdg = Tools::angle(this->start_place, this->end_place);
    return hdg;
}

std::string Runway::get_name() {
    return this->name;
}

bool Runway::operator ==(std::string name) {
    return this->name == name;
}

bool Runway::operator ==(Runway& runway) {
    return this->name == runway.get_name();
}

double Runway::get_glidepath() {
	return 3.0;
}
