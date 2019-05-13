#ifndef DRAWABLE_INPUT
#define DRAWABLE_INPUT

#include "drawable_element.hpp"

class Drawable_input : public Drawable_element {
public:
	Drawable_input(std::string value, std::string t_name, std::string t_class, std::string t_id);
	Drawable_input(std::string value, std::string t_name, std::set <std::string> classes, std::string t_id);
	void set_value(std::string value);
	std::string get_value();
private:
	std::string value;
};

#endif
