#include "drawable_element.hpp"
Drawable_element::Drawable_element(std::string t_name, std::string t_class, std::string t_id) : name(t_name), s_class(t_class), id(t_id) { }

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
	return this->s_class;
}

std::string Drawable_element::get_name() {
	return this->name;
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
		this->style.set_attribute("shape", (*style).get_shape());
		
		++style;
	 }
}

