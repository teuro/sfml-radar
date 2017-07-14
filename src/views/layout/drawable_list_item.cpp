#include "drawable_list_item.hpp"

Drawable_list_item::Drawable_list_item(std::string cnt, std::string t_class, std::string t_id) : Drawable_element("li", t_class, t_id), content(cnt) { }

Drawable_list_item::Drawable_list_item(std::string t_name, std::list <std::string> classes, std::string t_id) : Drawable_element(t_name, classes, t_id) { }

std::string Drawable_list_item::get_content() {
	return this->content;
}