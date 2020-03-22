#include "altitude_clearance.hpp"

Altitude_clearance::Altitude_clearance(std::shared_ptr <Settings> s, double altitude) : Clearance(s) { 
	this->set_value(altitude);
}

Altitude_clearance::~Altitude_clearance() { }

double Altitude_clearance::get_value() {
	return this->altitude;
}

void Altitude_clearance::set_value(double altitude) {
	if (this->check_value(this->settings->clearance_height_lower, this->settings->clearance_height_upper, altitude)) {
		this->altitude = altitude;
		
		this->return_message = "Clearance altitude " + Tools::tostr(altitude) + " is between limits";
	} else {
		this->return_message = "Altitude should be between " + Tools::tostr(this->settings->clearance_height_lower) + " and " + Tools::tostr(this->settings->clearance_height_upper);
	}
}

std::string Altitude_clearance::get_message() {
	return this->return_message;
}