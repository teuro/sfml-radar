#ifndef SPEED_CLEARANCE_HPP
#define SPEED_CLEARANCE_HPP

#include "clearance.hpp"

class Speed_clearance : public Clearance {
	double speed;
public:
	Speed_clearance(std::shared_ptr <Settings> s, double speed);
	~Speed_clearance();
	
	virtual double get_value();
	virtual void set_value(double speed);
	virtual std::string get_message();
};

#endif