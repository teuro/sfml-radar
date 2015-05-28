#ifndef _TOOLS_HPP
#define _TOOLS_HPP

#include <cmath>
#include <sstream>
#include <vector>
#include <cstdlib>

#include <algorithm>
#include <functional>
#include <cctype>
#include <locale>

#include "../models/coordinate.hpp"
#include "../models/point.hpp"

namespace Tools {
    double deg2rad(double deg);
    double rad2deg(double rad);

	double distanceNM(Coordinate& a, Coordinate& b);
	double distanceNM(double px, double zoom, double screen_width = 800);
	double angle(Coordinate& a, Coordinate& b);

	double distancePX(Point& a, Point& b);
	double distancePX(double nm, double zoom, double screen_width = 800);
	double angle(Point& a, Point& b);
	double angle(Coordinate& a, Coordinate& b);

	Point calculate(Point& cp, Coordinate& a, Coordinate& b, int zoom);
	Point calculate(Point& sp, double angle, double length, bool rad, bool turn = false);
	Coordinate calculate(Coordinate& cp, double angle, double length);
	bool is_match(std::string a, std::string b);

	int rnd(int a, int b);

	template <class T>
	std::string tostr(T a) {
        std::ostringstream os;

        os << a;

        return os.str();
	}

	template <class T>
	int toint(T a) {
        std::istringstream is(a);
        int tmp;

        is >> tmp;

        return tmp;
	}

	template <class T>
	T tonumber(std::string a) {
	    //std::clog << "Tools::tonumber(" << a << ")" << std::endl;
        std::istringstream is(a);
        T tmp;

        is >> tmp;

        return tmp;
	}


	bool on_area(Point& mouse, Point& aircraft);
	bool on_area(Coordinate& a, Coordinate& b);

	std::vector <std::string> split(std::string delimiter, std::string input);
	enum function_keys {UP = 1, DOWN, LEFT, RIGHT, NONE = -1};

	std::string ltrim(std::string s);
	std::string rtrim(std::string s);
	std::string trim(std::string s);

    double calculate_backwind(double wind, double runway);
    double get_PI();
}

#endif // _TOOLS_HPP
