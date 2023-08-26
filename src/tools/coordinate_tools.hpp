#ifndef _COODINATE_TOOLS_HPP
#define _COODINATE_TOOLS_HPP

#include <cmath>

#include <tools/tools.hpp>
#include "../models/coordinate/coordinate.hpp"

namespace Tools {
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
		* on_area
		* Checks if plane is inside of an area
		* @param Coordinate a
		* @param Coordinate b
		* @return bool
	**/
	
	bool on_area(Coordinate& a, Coordinate& b);
	
	/**
		* distanceNM 
		* Calculates distance between two coordinate points
		* @parameter Coordinate a
		* @parameter Coordinate b
		* @return double
	**/
	
	double distanceNM(Coordinate& a, Coordinate& b);
	
	/**
		* angle
		* give angle between two coordinate point 
		* angle is anticlockwise by default but You may choose clockwise 
		* @parameter Coordinate a
		* @parameter Coordinate b
		* @return double
	**/
	
	double angle(Coordinate& a, Coordinate& b);
}

#endif