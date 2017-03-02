#include "drawable_list.hpp"

Drawable_list::Drawable_list(std::string t_name, std::string t_class, std::string t_id) : Drawable_element(t_name, t_class, t_id) { }
Drawable_list::Drawable_list(std::string t_name, std::list <std::string> classes, std::string t_id) : Drawable_element(t_name, classes, t_id) { }
Drawable_list::Drawable_list() { }

void Drawable_list::add_element(std::string content) {
	this->elements.push_back(content);
}

void Drawable_list::clear_content() {
	this->elements.clear();
}

std::list <std::string> Drawable_list::get_elements() {
	return this->elements;
}
