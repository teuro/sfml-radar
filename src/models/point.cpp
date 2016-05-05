#include "point.hpp"

Point::Point(int x, int y) {
	this->x = x;
	this->y = y;
}

Point::~Point() { }

int Point::get_x() {
	return this->x;
}

int Point::get_y() {
	return this->y;
}

void Point::set_x(int x) {
	this->x = x;
}

void Point::set_y(int y) {
	this->y = y;
}

void Point::set_place(int x, int y) {
    this->x = x;
    this->y = y;
}

void Point::change_y(int amount) {
    this->y += amount;
}

void Point::change_x(int amount) {
    this->x += amount;
}

std::ostream& operator << (std::ostream& stream, const Point& point) {
    stream << "(" << point.x << ", " << point.y << ")";
    return stream;
}
