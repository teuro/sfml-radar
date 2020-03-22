#ifndef HEADING_CLEARANCE_HPP
#define HEADING_CLEARANCE_HPP

#include "clearance.hpp"

class Heading_clearance : public Clearance {
	double heading;
	int direction;
public:
	Heading_clearance(std::shared_ptr <Settings> s, double heading, int direction);
	~Heading_clearance();
	
	virtual double get_value();
	virtual void set_value(double heading);
	virtual std::string get_message();
	
	int get_direction();
};

#endif