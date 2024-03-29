#include "clickable.hpp"

Clickable::Clickable(std::string t_name, std::string t_class, std::string t_id, std::string cnt, std::shared_ptr <Settings> s) : Drawable_element(t_name, t_class, t_id), content(cnt), settings(s) { }

Clickable::~Clickable() { }

std::string Clickable::get_name() {
	return this->content;
}