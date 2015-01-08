#include "tools.hpp"

namespace Tools {
	const double PI = 3.14151927;
	const double earth_radius = 3440;
}

double Tools::deg2rad(double deg) {
	return deg * (Tools::PI / 180.0);
}

double Tools::rad2deg(double rad) {
    return (rad * 180.0) / Tools::PI;
}

double Tools::distanceNM(Coordinate& a, Coordinate& b) {
    double lat1 = a.get_latitude();
    double lat2 = b.get_latitude();
    double lon1 = a.get_longitude();
    double lon2 = b.get_longitude();

    double dlong = deg2rad(lon2 - lon1);
    double dlat  = deg2rad(lat2 - lat1);

    // Haversine formula:
    double a1 = std::sin(dlat/2)*std::sin(dlat/2) + std::cos(lat1)*std::cos(lat2)*std::sin(dlong/2)*std::sin(dlong/2);
    double c = 2 * std::atan2(std::sqrt(a1), std::sqrt(1-a1));
    double d = earth_radius * c;

    return d;
}

double Tools::distancePX(Point& a, Point& b) {
    int dx = std::abs(a.get_x() - b.get_x());
    int dy = std::abs(a.get_y() - b.get_y());

    return std::sqrt(std::pow(dx, 2.0) + std::pow(dy, 2.0));
}

double Tools::distanceNM(double px, double zoom, double screen_width) {
    return (px * zoom / screen_width);
}

double Tools::distancePX(double nm, double zoom, double screen_width) {
    return (screen_width * nm) / zoom;
}

double Tools::angle(Point& a, Point& b) {
    int dx = a.get_x() - b.get_x();
    int dy = a.get_y() - b.get_y();

    double tmp_angle = std::atan2(dy, dx);

    return tmp_angle;
}

double Tools::angle(Coordinate& a, Coordinate& b) {
	double tmp_x = b.get_latitude() - a.get_latitude();
	double tmp_y = b.get_longitude() - a.get_longitude();

    double tmp_angle = std::atan2(tmp_y, tmp_x);

	return tmp_angle;
}

Point Tools::calculate(Point& cp, Coordinate& a, Coordinate& b, int zoom) {
    /**
    std::clog << "Point& cp = " << cp.get_x() << ", " << cp.get_y() << std::endl;
    std::clog << "Coordinate& a = " << a.get_latitude() << ", " << a.get_longitude() << std::endl;
    std::clog << "Coordinate& b = " << b.get_latitude() << ", " << b.get_longitude() << std::endl;
    std::clog << "zoom = " << zoom << std::endl;
    **/
    double tmp_l = distanceNM(a, b);
    //std::clog << "Distance between " << a.get_latitude() << ", " << a.get_longitude() << " and " << b.get_latitude() << ", " << b.get_longitude() << " is " << tmp_l << " nm" << std::endl;
    double tmp_a = angle(a, b);
    //std::clog << "Bearing is " << tmp_a * rad2deg() << " degrees " << std::endl;

    int distance_pixels = (2 * cp.get_x() * tmp_l) / zoom;
    //std::clog << "Distance in pixels is " << distance_pixels << std::endl;

	return calculate(cp, tmp_a, distance_pixels, true, true);
}

Coordinate Tools::calculate(Coordinate& cp, double bearing, double d) {
    //std::clog << "Tools::calculate(" << cp.get_latitude() << ", " << cp.get_longitude() << ", " << bearing << ", " << d << std::endl;
    double lat1 = deg2rad(cp.get_latitude());
    double lon1 = deg2rad(cp.get_longitude());

    double lat2 = std::asin(std::sin(lat1) * std::cos(d / earth_radius) + std::cos(lat1) * std::sin(d / earth_radius) * std::cos(bearing));
    double lon2 = lon1 + std::atan2(std::sin(bearing) * std::sin(d / earth_radius) * std::cos(lat1), std::cos(d / earth_radius) - std::sin(lat1) * std::sin(lat2));

    lat2 = rad2deg(lat2);
    lon2 = rad2deg(lon2);

    Coordinate tmp(lat2, lon2);

    return tmp;
}

bool Tools::on_area(Point& mouse, Point& aircraft) {
    bool x = mouse.get_x() > (aircraft.get_x()-5) && mouse.get_x() < (aircraft.get_x()+5);
    bool y = mouse.get_y() > (aircraft.get_y()-5) && mouse.get_y() < (aircraft.get_y()+5);

    return (x && y);
}

std::vector <std::string> Tools::split(std::string delimiter, std::string input) {
    std::vector <std::string> tmp_cells;
    while (true) {
        std::string::size_type pos = input.find(delimiter, 0);

        if (pos == std::string::npos) {
            tmp_cells.push_back(input);
            return tmp_cells;
        }

        tmp_cells.push_back(input.substr(0, pos));
        input.erase(0, pos + 1);
    }
}

Point Tools::calculate(Point& sp, double bearing, double length, bool rad, bool turn) {
    if (!rad) {
        bearing = deg2rad(bearing);
    }

    if (turn) {
        bearing -= deg2rad(90.0);
    }

    double tmpx = sp.get_x() + std::cos(bearing) * length;
    double tmpy = sp.get_y() + std::sin(bearing) * length;

    Point tmp(tmpx, tmpy);
    return tmp;
}

int Tools::rnd(int a, int b) {
    return (a + std::rand() % (a + b));
}

std::string Tools::ltrim(std::string s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
    return s;
}

std::string Tools::rtrim(std::string s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
    return s;
}

std::string Tools::trim(std::string s) {
    return ltrim(rtrim(s));
}
