#ifndef _COORDINATES_HPP
#define _COORDINATES_HPP

#include <iostream>
#include <stdexcept>

/**
    * Coordinate
    * Represent places to all game objects.
    * There are no linking to screen coordinates.
    * Objects place on screen is calculated for every frame
**/

class Coordinate {
public:
	Coordinate(double latitude, double longitude);
	Coordinate(double latitude_degrees, double latitude_minutes, double latitude_seconds, double longitude_degrees, double longitude_minutes, double longitude_seconds);
	Coordinate() { }
	~Coordinate();
	
	friend std::ostream& operator << (std::ostream& virta, const Coordinate& coord);

    /**
        * get_latitude
        * Receive one double type value
        * @param none
        * @return double
    **/
	double get_latitude();

	/**
        * get_longitude
        * Receive one double type value
        * @param none
        * @return double
    **/
	double get_longitude();

    /**
        * set_latitude
        * Set new latitude value
        * @param double latitude
        * @return none
        * @throw std::logic_error
    **/
	void set_latitude(double latitude);

	/**
        * set_longitude
        * Set new longitude value
        * @param double latitude
        * @return none
        * @throw std::logic_error
    **/
	void set_longitude(double longitude);
protected:
private:
	double longitude;
	double latitude;
};


#endif // _COORDINATES_HPP
