#ifndef _POINT_TOOLS_HPP
#define _POINT_TOOLS_HPP

#include <cmath>

#include "tools.hpp"
#include "../models/coordinate/point.hpp"

namespace Tools {
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
	
	double angle(Point& a, Point& b);
	
	/**
		* calculate_midpoint
		* Calculates midpoint between two points
		* @param Point 
		* @param Point 
		* @return Point
	**/
	
	Point calculate_midpoint(Point& a, Point& b);
}

#endif