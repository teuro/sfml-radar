#ifndef _DRAWABLE_RUNWAY_ELEMENT
#define _DRAWABLE_RUNWAY_ELEMENT

#include "drawable_element.hpp"

class Drawable_Runway_Element : public Drawable_element {
public:
	Drawable_Runway_Element(Point place_begin, Point place_end, Point rwya);
	
	Point& get_begin();
	Point& get_end();
	Point& get_approach();
private:
	Point begin;
	Point end;
	Point approach;
};

#endif