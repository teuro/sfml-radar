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
#include <stdexcept>
#include <string>

/**
	* Tools 
	* This contain many helper functions. This namespace has only few variables which are constant values.
	* Do not add any mutable variables, because there no guarantee, that those variables are visible.
**/

namespace Tools {
	extern const double PI;
	extern const double earth_radius;
	extern const double milestofeets;
	
	/**
		* fix_angle
		* if angle is bigger than 2 * PI rad returns between 0 and 2 * PI
		* @param double 
		* @return duoble
	**/
	
	double fix_angle(double angle);
	
	/**
		* function keys declared
	**/
	
	enum function_keys {UP = 1, DOWN, LEFT, RIGHT, NONE = -1};
	
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
		* nm2ft
		* Converts nautical miles to feets
		* @param double miles
		* @return double
	**/
	
	double nm2ft(double nm);
	
	
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
	
	/**
		* round_nearest
		* Returns nearest multiply integer number
		* @param int number
		* @param int multiply
		* @return int nearest
		* @example round_nearest(105, 10) returns 110
		* @example round_nearest(1503, 100) returns 1500
	**/
	
	int round_nearest(int number, int multiply);
	int round_nearest(double number, int multiply);
}

#endif // _TOOLS_HPP
