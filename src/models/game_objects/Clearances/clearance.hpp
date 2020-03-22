#ifndef CLEARANCE_HPP
#define CLEARANCE_HPP

#include <memory>

#include "../../../tools/settings.hpp"

class Clearance {
protected:
	std::string return_message;
	virtual bool check_value(double lower_limit, double upper_limit, double value);
	std::shared_ptr <Settings> settings;
	Clearance(std::shared_ptr <Settings> s);
	virtual std::string get_message() = 0;
};

#endif