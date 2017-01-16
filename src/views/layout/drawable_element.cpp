#include "drawable_element.hpp"
Drawable_element::Drawable_element(std::string t_name, std::string t_class, std::string t_id) : name(t_name), s_class(t_class), id(t_id) { 
	this->styles["text-color"] = 25648;
	this->styles["background-color"] = -1;
}

Drawable_element::~Drawable_element() { }

void Drawable_element::set_style(std::string key, int value) {
	this->styles[key] = value;
}

void Drawable_element::set_style(Style& style) {
	this->styles["left"] = style.get_place().get_x();
	this->styles["top"] = style.get_place().get_y();
	this->styles["width"] = style.get_width();
	this->styles["height"] = style.get_height();
	this->styles["text-color"] = style.get_text_color();
	this->styles["background-color"] = style.get_background_color();
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