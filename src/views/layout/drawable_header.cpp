#include "drawable_header.hpp"

Header::Header(std::string cnt, std::string t_name, std::string t_class, std::string t_id) : Drawable_element(t_name, t_class, t_id), content(cnt) { }
Header::Header(std::string cnt, std::string t_name, std::list <std::string> classes, std::string t_id) : Drawable_element(t_name, classes, t_id), content(cnt) { }

std::string Header::get_content() {
	return this->content;
}

void Header::set_content(std::string cnt) {
	this->content = cnt;
}