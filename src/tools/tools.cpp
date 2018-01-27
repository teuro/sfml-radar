#include "tools.hpp"

namespace Tools {
	const double PI = 3.14151927;
	const double earth_radius = 3440;
	const double milestofeets = 6076.11549;
}

double Tools::fix_angle(double angle) {
	while (angle > 2 * Tools::get_PI()) {
		angle -= 2 * Tools::get_PI();
	}
	
	while (angle < 0) {
		angle += 2 * Tools::get_PI();
	}
	
	return angle;
}

double Tools::get_PI() {
    return Tools::PI;
}

void Tools::init_random() {
	std::srand(std::time(NULL));
}

int Tools::rnd(int a, int b) {
    return (a + std::rand() % (b - a));
}

double Tools::deg2rad(double deg) {
	return deg * (Tools::PI / 180.0);
}

double Tools::rad2deg(double rad) {
	rad = fix_angle(rad);
	
	return rad * 180.0 / Tools::PI;
}

int Tools::round_nearest(int number, int multiple) {
    if (multiple == 0) {
		return number;
	}

    int remainder = number % multiple;

    return number + multiple - remainder;
}

int Tools::round_nearest(double number, int multiple) {
	int t_number = (int) number;
    if (multiple == 0) {
		return number;
	}

    int remainder = t_number % multiple;

    return t_number + multiple - remainder;
}

double Tools::nm2ft(double nm) {
	return nm * milestofeets;
}

int Tools::hex2int(std::string hexa) {
	unsigned int color;
	std::stringstream ss;
	ss << std::hex << hexa;
	ss >> color;
	
	return color;
}