#include "drawable_list.hpp"

Drawable_list::Drawable_list(std::string t_name, std::string t_class, std::string t_id) : Drawable_element(t_name, t_class, t_id) { }
Drawable_list::Drawable_list(std::string t_name, std::set <std::string> classes, std::string t_id) : Drawable_element(t_name, classes, t_id) { }
Drawable_list::Drawable_list() { }

void Drawable_list::add_element(std::string content, std::string t_class, std::string t_id) {
	Drawable_list_item tmp(content, t_class, t_id);
	
	this->elements.push_back(tmp);
}

void Drawable_list::add_element(std::string content, std::set <std::string> classes, std::string t_id) {
	Drawable_list_item tmp(content, classes, t_id);
	
	this->elements.push_back(tmp);
}

void Drawable_list::clear_content() {
	this->elements.clear();
}

std::list <Drawable_list_item> Drawable_list::get_elements() {
	return this->elements;
}

std::string Drawable_list::get_max_length() {
	std::string max = "";
	
	std::list <Drawable_list_item> :: iterator sit = this->elements.begin();
	
	while (sit != this->elements.end()) {
		if (sit->get_content().length() > max.length()) {
			max = sit->get_content();
		}
		
		++sit;
	}
	
	return max;
}

void Drawable_list::set_class(std::string t_class) {
	this->classes.insert(t_class);
	
	std::list <Drawable_list_item> :: iterator sit = this->elements.begin();
	
	while (sit != this->elements.end()) {
		sit->set_class(t_class);
		
		++sit;
	}
}
