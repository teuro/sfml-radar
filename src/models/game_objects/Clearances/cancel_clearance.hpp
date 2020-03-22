#ifndef CANCEL_CLEARANCE_HPP
#define CANCEL_CLEARANCE_HPP

#include "clearance.hpp"

class Cancel_clearance : public Clearance {
	double speed;
public:
	Cancel_clearance(std::shared_ptr <Settings> s, double altitude);
	~Cancel_clearance();
	
	virtual double get_value();
	virtual void set_value(double altitude);
	virtual std::string get_message();
};

#endif