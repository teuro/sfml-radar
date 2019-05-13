#include "drawable_element.hpp"
Drawable_element::Drawable_element(std::string t_name, std::string t_class, std::string t_id) : name(t_name), id(t_id) { 
	this->classes.insert(t_class);
}

Drawable_element::Drawable_element(std::string t_name, std::set <std::string> t_classes, std::string t_id) : name(t_name), classes(t_classes), id(t_id) { }

Drawable_element::Drawable_element() { }

Drawable_element::~Drawable_element() { }

void Drawable_element::set_style(Style& stle) {
	this->styles.push_back(stle);
	this->calculate_styles();
}

Style& Drawable_element::get_style() {
	return this->style;
}

std::string Drawable_element::get_id() {
	return this->id;
}

std::string Drawable_element::get_class() {
	std::set <std::string> :: iterator it = this->classes.begin();
	std::string tmp;
	
	while (it != this->classes.end()) {
		tmp += (*it) + " ";
		++it;
	}
	
	return tmp;
}

std::set <std::string> Drawable_element::get_classes() {	
	return classes;
}

std::string Drawable_element::get_name() {
	return this->name;
}

void Drawable_element::set_class(std::string t_class) {
	this->classes.insert(t_class);
}

void Drawable_element::calculate_styles() {
	std::list <Style> :: iterator style = this->styles.begin();
	 
	while (style != this->styles.end()) {
		this->style.set_attribute("left", (*style).get_left());
		this->style.set_attribute("top", (*style).get_top());
		this->style.set_attribute("width", (*style).get_width());
		this->style.set_attribute("height", (*style).get_height());
		this->style.set_attribute("color", (*style).get_text_color());
		this->style.set_attribute("background-color", (*style).get_background_color());
		this->style.set_attribute("border", (*style).get_border_color());
		this->style.set_attribute("shape", (*style).get_shape());
		this->style.set_attribute("class", (*style).get_class());
		this->style.set_attribute("id", (*style).get_id());
		
		++style;
	 }
}

std::string Drawable_element::get_max_length() {
	return "";
}

void Drawable_element::clear_classes() {
	this->classes.clear();
}