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
	/**
		* deg2rad
		* Converts degrees to radians
		* @parameter double deg
		* @return double
	**/
    double deg2rad(double deg);
	
	/**
		* rad2deg
		* converts radians to degrees
		* @parameter double rad
		* @return double
	**/
    double rad2deg(double rad);

	/**
		* distanceNM 
		* Calculates distance between two coordinate points
		* @parameter Coordinate a
		* @parameter Coordinate b
		* @return double
	**/
	double distanceNM(Coordinate& a, Coordinate& b);
	
	/**
		* distanceNM
		* Converts pixels to nautical miles 
		* @parameter double px
		* @parameter double zoom
		* @parameter double screen_width
		* @return double
	**/
	double distanceNM(double px, double zoom, double screen_width = 800);
	
	/**
		* distancePX
		* Calculates distance between two screen points
		* @paramterer Point a
		* @paramterer Point b
		* @return double
	**/
	double distancePX(Point& a, Point& b);
	
	/**
		* distanceP´X
		* Converts nautcal miles to pixels
		* @parameter double nm
		* @parameter double zoom
		* @parameter double screen_width
		* @return double
	**/
	double distancePX(double nm, double zoom, double screen_width = 800);
	
	/**
		* angle
		* Calculates angle between two
	**/
	
	double angle(Point& a, Point& b);
	
	/**
		* angle
		* give angle between two coordinate point 
		* angle is anticlockwise by default but You may choose clockwise 
		* @parameter Coordinate a
		* @parameter Coordinate b
		* @return double
	**/
	
	double angle(Coordinate& a, Coordinate& b);
	
	/**
		* calculate
		* Calculates new 
	**/

	Point calculate(Point& center_point_screen, Coordinate& center_point_map, Coordinate& target, int zoom);
	Point calculate(Point& sp, double angle, double length, bool rad);
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

    double get_PI();
}

#endif // _TOOLS_HPP
