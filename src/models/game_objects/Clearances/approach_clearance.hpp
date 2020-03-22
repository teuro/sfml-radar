#ifndef APPROACH_CLEARANCE_HPP
#define APPROACH_CLEARANCE_HPP

#include "clearance.hpp"

class Approach_clearance : public Clearance {
	double speed;
public:
	Approach_clearance(std::shared_ptr <Settings> s, std::string runway);
	~Approach_clearance();
	
	virtual double get_value();
	virtual void set_value(std::string runway);
	virtual std::string get_message();
};

#endif