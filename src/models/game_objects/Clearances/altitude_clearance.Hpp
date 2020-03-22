#ifndef ALTIDE_CLEARANCE_HPP
#define ALTIDE_CLEARANCE_HPP

#include "clearance.hpp"

class Altitude_clearance : public Clearance {
	double altitude;
public:
	Altitude_clearance(std::shared_ptr <Settings> s, double altitude);
	~Altitude_clearance();
	
	double get_value();
	void set_value(double altitude);
	virtual std::string get_message();
};

#endif