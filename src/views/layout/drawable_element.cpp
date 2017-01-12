#include "drawable_element.hpp"

Drawable_element::Drawable_element(std::string i, Point p) : id(i), place(p) { }

Drawable_element::~Drawable_element() { }

void Drawable_element::set_id(std::string id) {
	this->id = id;
}

void Drawable_element::set_place(Point& p) {
	this->place = p;
}

std::string Drawable_element::get_id() {
	return this->id;
}

Point& Drawable_element::get_place() {
	return this->place;
}