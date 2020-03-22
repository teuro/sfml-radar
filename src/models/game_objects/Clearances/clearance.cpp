#include "clearance.hpp"

Clearance::Clearance(std::shared_ptr <Settings> s) : settings(s) { }

bool Clearance::check_value(double lower_limit, double upper_limit, double value) {
	return (value >= lower_limit && value <= upper_limit);
}