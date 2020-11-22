#include "Drawable_Runway_Element.hpp"

Drawable_Runway_Element::Drawable_Runway_Element(Point place_begin, Point place_end, Point place_approach) : Drawable_element("runway") {
	this->begin = place_begin;
	this->end = place_end;
	this->approach = place_approach;
}
	
Point& Drawable_Runway_Element::get_begin() {
	return this->begin;
}

Point& Drawable_Runway_Element::get_end() {
	return this->end;
}

Point& Drawable_Runway_Element::get_approach() {
	return this->approach;
}