#ifndef DRAWABLE_ELEMENT
#define DRAWABLE_ELEMENT

#include <string>
#include "../../models/point.hpp"

struct Drawable_element {
private:
	std::string id;
	Point place;
protected:
	Drawable_element(std::string i, Point p);
	~Drawable_element();
public:
	void set_id(std::string id);
	void set_place(Point& p);
	
	std::string get_id();
	Point& get_place();
};

#endif