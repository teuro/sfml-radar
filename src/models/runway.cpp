#include "runway.hpp"

Runway::Runway(std::string n, Coordinate s, Coordinate e) : name(n), start_place(s), end_place(e), init_heading(220), turning_altirude(550) {
    //std::clog << "Runway " << this->name << " created" << std::endl;
}

Runway::~Runway() { }

Coordinate& Runway::get_start_place() {
    return this->start_place;
}

Coordinate& Runway::get_end_place() {
    return this->end_place;
}

double Runway::get_length() {
    return Tools::distanceNM(this->start_place, this->end_place);
}

double Runway::get_heading() {
	std::clog << "Runway::get_heading of " << this->name << std::endl;
    double hdg = Tools::angle(this->start_place, this->end_place);

    return hdg;
}

std::string Runway::get_name() {
    return this->name;
}

bool Runway::operator ==(std::string name) {
    return this->name == name;
}
