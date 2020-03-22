#include "speed_clearance.hpp"

Speed_clearance::Speed_clearance(std::shared_ptr <Settings> s, double spd) : Clearance(s) { 
	this->set_value(spd);
}
Speed_clearance::~Speed_clearance() { }

double Speed_clearance::get_value() {
	return this->speed;
}

void Speed_clearance::set_value(double spd) {
	if (this->check_value(this->settings->clearance_speed_lower, this->settings->clearance_speed_upper, spd)) {
		this->speed = spd;
		
		this->return_message = "Clearance speed " + Tools::tostr(spd) + " is between limits";
	} else {
		this->return_message = "Speed should be between " + Tools::tostr(this->settings->clearance_speed_lower) + " and " + Tools::tostr(this->settings->clearance_speed_upper);
	}
}

std::string Speed_clearance::get_message() {
	return this->return_message;
}