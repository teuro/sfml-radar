#ifndef EXPECT_CLEARANCE_HPP
#define EXPECT_CLEARANCE_HPP

#include "clearance.hpp"

class Expect_clearance : public Clearance {
	std::string runway;
public:
	Expect_clearance(std::shared_ptr <Settings> s, std::string runway);
	~Expect_clearance();
	
	virtual void set_value(std::string runway);
	virtual std::string get_message();
	
	std::string get_value();
};

#endif