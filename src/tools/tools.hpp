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
#include <random>
#include <iostream>

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
		* converts string to number. This is just more flexibile instead of use toint()
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
		* hex2int
		* Converts #... format hex string to int
		* represent color code
		* @param std::string
		* @return int
	**/
	
	int hex2int(std::string hexa);
	
	/**
		* linear_random
		* Returns pseudo random integer
		* @param int a lower bound
		* @param int b upper bound
		* @return int
	**/
		
	int linear_random(int a, int b);
	
	/**
		* normal_distribution
		* Return normal distribution
		* @param double mean 
		* @param double variation 
		* @return double
	**/
	
	double normal_distribution(double mean, double variation);
	
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
		int place = Tools::linear_random(0, (int)input.size()-1);
		
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
	
	template <typename Iter, typename RandomGenerator>
	Iter select_randomly(Iter start, Iter end, RandomGenerator& g) {
		std::uniform_int_distribution<> dis(0, std::distance(start, end) - 1);
		std::advance(start, dis(g));
		return start;
	}

	template <typename Iter>
	Iter select_randomly(Iter start, Iter end) {
		static std::random_device rd;
		static std::mt19937 gen(rd());
		return select_randomly(start, end, gen);
	}
}

#endif // _TOOLS_HPP
