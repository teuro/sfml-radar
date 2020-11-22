#ifndef _DRAWABLE_NAVPOINT_ELEMENT
#define _DRAWABLE_NAVPOINT_ELEMENT

#include "drawable_element.hpp"

class Drawable_Navpoint_Element : public Drawable_element {
public:
	Drawable_Navpoint_Element(Point place, std::string name);
	
	Point& get_place();
	std::string get_name();
private:
	Point place;
	std::string name;
};

#endif