#include "drawable_navpoint_element.hpp"

Drawable_Navpoint_Element::Drawable_Navpoint_Element(Point place, std::string name) : Drawable_element("navpoint") {
	this->place = place;
	this->name = name;
}
	
std::string Drawable_Navpoint_Element::get_name() {
	return this->name;
}

Point& Drawable_Navpoint_Element::get_place() {
	return this->place;
}