#include "expect_clearance.hpp"

Expect_clearance::Expect_clearance(std::shared_ptr <Settings> s, std::string rwy): Clearance(s), runway(rwy) { }

Expect_clearance::~Expect_clearance() { }

void Expect_clearance::set_value(std::string rwy) {
	this->runway = rwy;
}

std::string Expect_clearance::get_message() {
	return this->return_message;
}

std::string Expect_clearance::get_value() {
	return this->runway;
}