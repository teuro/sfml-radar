#include "drawable_image.hpp"

Image::Image(std::string src, std::string t_name, std::string t_class, std::string t_id) : Drawable_element(t_name, t_class, t_id), source(src) { }

std::string Image::get_source() {
	return this->source;
}
	
void Image::set_source(std::string src) {
	this->source = src;
}