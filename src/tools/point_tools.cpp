#include "point_tools.hpp"

Point Tools::calculate(Point& sp, double bearing, double length) {
	bearing = Tools::fix_angle(bearing);
	
    double tmpx = sp.get_x() + std::cos(bearing) * length;
    double tmpy = sp.get_y() - std::sin(bearing) * length;
	
	Point tmp(tmpx, tmpy);

    return tmp;
}

bool Tools::on_area(Point& mouse, Point& target, int buffer) {
	bool x = mouse.get_x() > (target.get_x() - buffer) && mouse.get_x() < (target.get_x() + buffer);
	bool y = mouse.get_y() > (target.get_y() - buffer) && mouse.get_y() < (target.get_y() + buffer);

    return (x && y);
}

bool Tools::on_area(Point& mouse, Point& target, int width, int height) {
	bool x = mouse.get_x() > (target.get_x()) && mouse.get_x() < (target.get_x() + width);
	bool y = mouse.get_y() > (target.get_y()) && mouse.get_y() < (target.get_y() + height);

    return (x && y);
}

bool Tools::on_area(Point& mouse, Point& target_lu, Point& target_rd) {
	bool x = mouse.get_x() > target_lu.get_x() && mouse.get_x() < target_rd.get_x();
	bool y = mouse.get_y() > target_lu.get_y() && mouse.get_y() < target_rd.get_y();
	
	return (x && y);
}

double Tools::distancePX(Point& a, Point& b) {
    int dx = std::abs(a.get_x() - b.get_x());
    int dy = std::abs(a.get_y() - b.get_y());

    return std::sqrt(std::pow(dx, 2.0) + std::pow(dy, 2.0));
}

double Tools::angle(Point& a, Point& b) {
    int dx = a.get_x() - b.get_x();
    int dy = a.get_y() - b.get_y();

	return std::atan2(dy, dx);
}

Point Tools::calculate_midpoint(Point& a, Point& b) {
	int x = std::abs((a.get_x() + b.get_x()) / 2);
	int y = std::abs((a.get_y() + b.get_y()) / 2);
	
	Point t(x, y);
	
	return t;
}