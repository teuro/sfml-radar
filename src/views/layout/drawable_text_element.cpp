#include "drawable_text_element.hpp"

Drawable_text_element::Drawable_text_element(std::string cnt) : content(cnt) { }

std::string Drawable_text_element::get_content() {
	return this->content;
}