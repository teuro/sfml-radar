#include "tools.hpp"

namespace Tools {
	const double PI = 3.14151927;
	const double earth_radius = 3440;
}

double Tools::get_PI() {
    return Tools::PI;
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

   return (double)(std::fmod(2 * PI + tmp_angle, 2 * PI));
}

double Tools::angle(Coordinate& a, Coordinate& b) {
	double fLat = a.get_latitude();
    double fLng = a.get_longitude();
    double tLat = b.get_latitude();
    double tLng = b.get_longitude();
	
	double y = std::sin(tLng-fLng) * std::cos(tLat);
	double x = std::cos(fLat)*std::sin(tLat) - std::sin(fLat)*std::cos(tLat)*std::cos(tLng-fLng);
	double brng = std::atan2(y, x);
	
	brng += PI / 2.0;
	
	return (double)(std::fmod(2 * PI + brng, 2 * PI));
}

Point Tools::calculate(Point& sp, double bearing, double length, bool rad) {
	if (!rad) {
        bearing = deg2rad(bearing);
    }
	
	while (bearing < 0.0) {
		bearing += 2 * PI;
	}
	
	while (bearing > 2 * PI) {
		bearing -= 2 * PI;
	}
	
    double tmpx = sp.get_x() + std::cos(bearing) * length;
    double tmpy = sp.get_y() - std::sin(bearing) * length;
	
	Point tmp(tmpx, tmpy);

    return tmp;
}

Point Tools::calculate(Point& center_point_screen, Coordinate& center_point_map, Coordinate& target, int zoom) {
    double tmp_length = Tools::distanceNM(center_point_map, target);
    //std::clog << "Distance between " << center_point_map.get_latitude() << ", " << center_point_map.get_longitude() << " and " << target.get_latitude() << ", " << target.get_longitude() << " is " << tmp_l << " nm" << std::endl;
    
	double tmp_angle = Tools::angle(center_point_map, target);
	//std::clog << "Bearing is " << rad2deg(tmp_angle) << " degrees " << std::endl;

    int distance_pixels = Tools::distancePX(tmp_length, zoom);
    //std::clog << "Distance in pixels is " << distance_pixels << std::endl;

	return Tools::calculate(center_point_screen, tmp_angle, distance_pixels, true);
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

bool Tools::on_area(Point& mouse, Point& target, int buffer) {
	bool x = mouse.get_x() > (target.get_x() - buffer) && mouse.get_x() < (target.get_x() + buffer);
	bool y = mouse.get_y() > (target.get_y() - buffer) && mouse.get_y() < (target.get_y() + buffer);

    return (x && y);
}

bool Tools::on_area(Point& mouse, Point& target_lu, Point& target_rd) {
	bool x = mouse.get_x() > target_lu.get_x() && mouse.get_x() < target_rd.get_x();
	bool y = mouse.get_y() > target_lu.get_y() && mouse.get_y() < target_rd.get_y();
	
	return (x && y);
}

bool Tools::on_area(Coordinate& a, Coordinate& b) {
    return (distanceNM(a, b) < 0.3);
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

bool Tools::is_match(std::string a, std::string b) {
	for (unsigned int i = 0; i < a.length(); ++i) {
		if (a[i] != b[i]) {
			return false;
		}
	}
	
	return true;
}

void Tools::init_random() {
	std::srand(std::time(NULL));
}
