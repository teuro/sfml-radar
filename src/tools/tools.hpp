#ifndef TOOLS_HPP
#define TOOLS_HPP

#include <cmath>
#include <sstream>
#include <vector>
#include <cstdlib>

#include <algorithm>
#include <functional>
#include <cctype>
#include <locale>
#include <cstdlib>
#include <ctime>
#include <map>

#include "../models/coordinate/coordinate.hpp"
#include "../models/coordinate/point.hpp"

/**
	* Tools 
	* This contain many helper functions. This namespace has only few variables which are constant values.
	* Do not add any mutable variables, because there no guarantee, that those variables are visible.
**/

namespace Tools {
	/**
		* fix_angle
		* if angle is bigger than 2 * PI rad returns between 0 and 2 * PI
		* @param double 
		* @return duoble
	**/
	
	double fix_angle(double angle);
	
	/**
		* calculate
		* Calculates new screen point
		* @param Point start_point
		* @param double angle radians from point
		* @param double length
		* @param bool rad
		* @return Point
	**/
	
	Point calculate(Point& start_point, double angle, double length);
	
	/**
		* calculate
		* Calculates new screen point
		* @param Coordinate start_point
		* @param double angle radians from point
		* @param double length
		* @param bool rad
		* @return Coordinate
	**/
	
	Coordinate calculate(Coordinate& start_point, double angle, double length);
	
	/**
		* is_match
		* Checks if two strings are similar
		* @param std::string a
		* @param std::string b
		* @return bool
	**/
	
	bool is_match(std::string a, std::string b);

	/**
		* tostr
		* converts numbers to string
		* @param T might bee any numbers
		* @return std::string
	**/
	
	template <class T>
	std::string tostr(T a) {
        std::ostringstream os;

        os << a;

        return os.str();
	}
	
	/**
		* tostr
		* converts numbers to constant length string
		* @param T might bee any numbers
		* @param unsigned int length 
		* @return std::string
	**/
	
	template <class T>
	std::string tostr(T a, unsigned int length) {
        std::ostringstream os;

        os << a;
		
		std::string t = os.str();
		
		if (t.length() < length) {
			unsigned int required = length - t.length();
			std::string fill_zeros;
			
			for ( unsigned int i = 0; i < required; ++i) {
				fill_zeros += "0";
			}
			
			t = fill_zeros + t;
		}

        return t;
	}
	
	/**
		* totime
		* converts double to time with specific format
		* @param double milliseconds
		* @param std::string milliseconds
		* @description
		* Format follows PHP's similar functions. 
	**/
	
	std::string totime(double milliseconds, std::string format = "H:i:s");
	
	/**
		* on_area
		* Checks if mouse is inside of an area
		* @param Point mouse
		* @param Point target
		* @param int buffer
		* @return bool
	**/

	bool on_area(Point& mouse, Point& target, int buffer = 5);
	
	/**
		* on_area
		* Checks if mouse is inside of an area
		* @param Point mouse
		* @param Point target_left_upper
		* @param Point target_right_lower
		* @return bool
	**/
	
	bool on_area(Point& mouse, Point& target_left_upper, Point& target_right_lower);
	
	/**
		* on_area
		* Checks if mouse is inside of an area
		* @param Point mouse
		* @param int width
		* @param int height
		* @return bool
	**/
	
	bool on_area(Point& mouse, Point& target_left_upper, int width, int height);
	
	/**
		* on_area
		* Checks if plane is inside of an area
		* @param Coordinate a
		* @param Coordinate b
		* @return bool
	**/
	
	bool on_area(Coordinate& a, Coordinate& b);
	
	/**
		* split
		* explodes string to pieces
		* @param std::string delimiter
		* @param std::string input
		* @return std::vector <std::string>
	**/
	
	std::vector <std::string> split(std::string delimiter, std::string input);
	
	/**
		* function keys declared
	**/
	
	enum function_keys {UP = 1, DOWN, LEFT, RIGHT, NONE = -1};
	
	/**
		* ltrim
		* trims spaces from start of string
		* @param std::string s
		* @return std::string
	**/

	std::string ltrim(std::string s);
	
	/**
		* ltrim
		* trims spaces from end of string
		* @param std::string s
		* @return std::string
	**/
	
	std::string rtrim(std::string s);
	
	/**
		* ltrim
		* trims spaces from start and end of string
		* @param std::string s
		* @return std::string
	**/
	
	std::string trim(std::string s);
	
	/**
		* get_PI
		* returns constant value 3.1415...
		* @param void
		* @return double
	**/

    double get_PI();
	
	/**
		* init_randorm
		* initialize pseudo random generator
		* @param void
		* @return void
	**/
	
	void init_random();
	
	/**
		* replace
		* Loops throug map and make replace each loop
		* @param std::string original
		* @param std::map <std::string, std::string> repl
		* @return std::string
	**/
	
	std::string replace(std::string original, std::map <std::string, std::string> repl);
	
	/**
		* replace
		* Finds specific strign and replace it with other string
		* @param std::string original
		* @param std::string 
		* @param std::string 
		* @return std::string
	**/
	
	std::string replace(std::string original, std::string find, std::string replace);
	
	/**
		* calculate_midpoint
		* Calculates midpoint between two points
		* @param Point 
		* @param Point 
		* @return Point
	**/
	
	Point calculate_midpoint(Point& a, Point& b);
	/**
		* toint
		* converts string to int. This is just more easier instead of use tonumber<int>()
		* @param std::string
		* @return int
	**/
	
	template <class T>
	int toint(T a) {
        std::istringstream is(a);
        int tmp;

        is >> tmp;

        return tmp;
	}
	
	/**
		* tonumber
		* converts string to int. This is just more flexibile instead of use toint()
		* @param std::string
		* @return number
	**/

	template <class T>
	T tonumber(std::string a) {
	    //std::clog << "Tools::tonumber(" << a << ")" << std::endl;
        std::istringstream is(a);
        T tmp;

        is >> tmp;

        return tmp;
	}
	
		/**
		* rnd
		* Returns pseudo random integer
		* @param int a lower bound
		* @param int b upper bound
		* @return int
	**/
		
	int rnd(int a, int b);
	
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
		* distancePX
		* Calculates distance between two screen points
		* @paramterer Point a
		* @paramterer Point b
		* @return double
	**/
	double distancePX(Point& a, Point& b);
	
	/**
		* angle
		* Calculates angle between two screen points
		* @param Point& a reference point
		* @param Point& b reference point
		* @return double
	**/
	
	/**
		* nm2ft
		* Converts nautical miles to feets
		* @param double miles
		* @return double
	**/
	
	double nm2ft(double nm);
	
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
		* random_object
		* returns random object from vector
		* @param std::vector <T>
		* @return T
	**/
	
	template <class T>
	T random_object(std::vector <T> input) {
		int place = Tools::rnd(0, (int)input.size()-1);
		
		return input[place];
	}
	
	int round_nearest(int number, int multiply);
}
#endif // _TOOLS_HPP
