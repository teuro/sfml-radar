#include "coordinate_tools.hpp"

Coordinate Tools::calculate(Coordinate& cp, double bearing, double d) {
    double lat1 = deg2rad(cp.get_latitude());
    double lon1 = deg2rad(cp.get_longitude());

    double lat2 = std::asin(std::sin(lat1) * std::cos(d / earth_radius) + std::cos(lat1) * std::sin(d / earth_radius) * std::cos(bearing));
    double lon2 = lon1 + std::atan2(std::sin(bearing) * std::sin(d / earth_radius) * std::cos(lat1), std::cos(d / earth_radius) - std::sin(lat1) * std::sin(lat2));

    lat2 = rad2deg(lat2);
    lon2 = rad2deg(lon2);

    Coordinate tmp(lat2, lon2);

    return tmp;
}

bool Tools::on_area(Coordinate& a, Coordinate& b) {
    return (distanceNM(a, b) < 0.3);
}

double Tools::angle(Coordinate& a, Coordinate& b) {
	double fLat = deg2rad(a.get_latitude());
    double fLng = deg2rad(a.get_longitude());    
	double tLat = deg2rad(b.get_latitude());
    double tLng = deg2rad(b.get_longitude());
	
	double y = std::sin(tLng-fLng) * std::cos(tLat);
	double x = std::cos(fLat) * std::sin(tLat) - std::sin(fLat) * std::cos(tLat) * std::cos(tLng - fLng);
	double brng = std::atan2(y, x);
	
	return brng;
}

double Tools::distanceNM(Coordinate& a, Coordinate& b) {
    double lat1 = a.get_latitude();
    double lat2 = b.get_latitude();
    double lon1 = a.get_longitude();
    double lon2 = b.get_longitude();

    double dlong = deg2rad(lon2 - lon1);
    double dlat  = deg2rad(lat2 - lat1);

    double a1 = std::sin(dlat / 2.0) * std::sin(dlat / 2.0) + std::cos(lat1) * std::cos(lat2) * std::sin(dlong / 2.0) * std::sin(dlong / 2.0);
    double c = 2.0 * std::atan2(std::sqrt(a1), std::sqrt(1.0 - a1));
    double d = Tools::earth_radius * c;

    return d;
}