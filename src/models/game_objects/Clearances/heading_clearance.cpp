#include "heading_clearance.hpp"

Heading_clearance::Heading_clearance(std::shared_ptr <Settings> s, double heading, int drc) : Clearance(s), direction(drc) { 
	this->set_value(heading);
}

Heading_clearance::~Heading_clearance() { }

double Heading_clearance::get_value() {
	return this->heading;
}

void Heading_clearance::set_value(double heading) {
	this->heading = Tools::deg2rad(heading);
		
	this->return_message = "Heading " + Tools::tostr(heading) + " ok";
}

std::string Heading_clearance::get_message() {
	return this->return_message;
}

int Heading_clearance::get_direction(){
	return this->direction;
}