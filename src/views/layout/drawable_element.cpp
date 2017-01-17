#include "drawable_element.hpp"
Drawable_element::Drawable_element(std::string t_name, std::string t_class, std::string t_id) : name(t_name), s_class(t_class), id(t_id) { }

Drawable_element::~Drawable_element() { }

void Drawable_element::set_style(std::string key, int value) {
	if (value > 0 ) {
		this->styles[key] = value;
	}
}

void Drawable_element::set_style(Style& style) {
	this->set_style("left", style.get_place().get_x());
	this->set_style("top", style.get_place().get_y());
	this->set_style("width", style.get_width());
	this->set_style("height", style.get_height());
	this->set_style("text-color", style.get_text_color());
	this->set_style("background-color", style.get_background_color());
}

int Drawable_element::get_style(std::string key) {
	return this->styles[key];
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