#include "coordinate.hpp"

Coordinate::Coordinate(double latitude, double longitude) {
	this->latitude = latitude;
	this->longitude = longitude;
}

Coordinate::Coordinate(double latitude_degrees, double latitude_minutes, double latitude_seconds, double longitude_degrees, double longitude_minutes, double longitude_seconds) {
    this->latitude = latitude_degrees + (latitude_minutes / 60.0) + (latitude_seconds / 3600.0);
    this->longitude = longitude_degrees + (longitude_minutes / 60.0) + (longitude_seconds / 3600.0);
}

Coordinate::~Coordinate() { }

double Coordinate::get_latitude() {
	return this->latitude;
}

double Coordinate::get_longitude() {
	return this->longitude;
}

std::ostream& operator << (std::ostream& virta, const Coordinate& coord) {
	virta << "(" << coord.latitude << ", " << coord.longitude << ")";
	return virta;
}
