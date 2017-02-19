#include "drawable_input.hpp"

Drawable_input::Drawable_input(std::string val, std::string t_name, std::string t_class, std::string t_id) : Drawable_element(t_name, t_class, t_id), value(val) { }

void Drawable_input::set_value(std::string val) {
	this->value = val;
}

std::string Drawable_input::get_value() {
	return this->value;
}