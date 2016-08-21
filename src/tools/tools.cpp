#include "tools.hpp"

namespace Tools {
	const double PI = 3.14151927;
	const double earth_radius = 3440;
	static double scale_px = 1.0 / 200.0;
	static double scale_nm = 1 / scale_px;
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

double Tools::distanceNM(double px, double zoom) {
    return px * (scale_px * zoom);
}

double Tools::distancePX(double nm, double zoom) {
    return nm * (scale_nm / zoom);
}

double Tools::angle(Point& a, Point& b) {
    int dx = a.get_x() - b.get_x();
    int dy = a.get_y() - b.get_y();

	return std::atan2(dy, dx);
}

double Tools::angle(Coordinate& a, Coordinate& b) {
	double fLat = a.get_latitude();
    double fLng = a.get_longitude();
    double tLat = b.get_latitude();
    double tLng = b.get_longitude();
	
	double y = std::sin(tLng-fLng) * std::cos(tLat);
	double x = std::cos(fLat)*std::sin(tLat) - std::sin(fLat)*std::cos(tLat)*std::cos(tLng-fLng);
	double brng = std::atan2(y, x);
	
	return (double)(std::fmod(2.0 * PI + brng, 2.0 * PI));
}

Point Tools::calculate(Point& sp, double bearing, double length) {
	bearing = Tools::fix_angle(bearing);
	
    double tmpx = sp.get_x() + std::cos(bearing) * length;
    double tmpy = sp.get_y() - std::sin(bearing) * length;
	
	Point tmp(tmpx, tmpy);

    return tmp;
}

Point Tools::calculate(Point& centerpoint_screen, Coordinate& centerpoint_map, Coordinate& target, int zoom) {
    double tmp_length = Tools::distanceNM(centerpoint_map, target);
    
	double tmp_angle = Tools::CalcGeograpicAngle(Tools::angle(centerpoint_map, target));
	
    int distance_pixels = Tools::distancePX(tmp_length, zoom);
    
	return Tools::calculate(centerpoint_screen, tmp_angle, distance_pixels);
}

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

std::string Tools::replace(std::string original, std::map <std::string, std::string> repl) {
	std::map <std::string, std::string> :: iterator it;
	
    for (it = repl.begin(); it != repl.end(); ++it) {
		original = replace(original, it->first, it->second);
    }
	
	return original;
}

std::string Tools::replace(std::string original, std::string find, std::string replace) {
	std::string::size_type pos = original.find(find, 0);
		
	if (pos != std::string::npos) {
		std::string part1 = original.substr(0, pos);
		std::string part2 = original.substr(pos, find.length());
		std::string part3 = original.substr(pos + find.length());
		
		part2 = replace;
		
		original = part1 + part2 + part3;
	}
	
	return original;
}

std::string Tools::totime(double milliseconds, std::string format) {
	std::string time_string;
	int seconds = milliseconds / 1000;
	int hours = seconds / 3600;
	seconds -= hours * 3600;
	int minutes = seconds / 60;
	seconds -= minutes * 60;
	
	std::map <std::string, std::string> format_table;
	
	format_table["H"] = tostr(hours, 2);
	format_table["h"] = tostr(hours, 1);
	format_table["i"] = tostr(minutes, 2);
	format_table["m"] = tostr(minutes, 1);
	format_table["s"] = tostr(seconds, 2);
	format_table["t"] = tostr(seconds, 1);
	
	time_string = replace(format, format_table);
	
	return time_string;
}

double Tools::fix_angle(double angle) {
	while (angle > 2 * Tools::get_PI()) {
		angle -= 2 * Tools::get_PI();
	}
	
	while (angle < 0) {
		angle += 2 * Tools::get_PI();
	}
	
	return angle;
}

double Tools::CalcGeograpicAngle(double arith) {
	if (arith > ((PI / 2.0) && arith < (2 * PI))) {
		return ((2 * PI) - arith + (PI / 2.0));
	} 
	
	return (arith * (-1.0) + (PI / 2.0));
}