#include "style.hpp"

Style::Style() { 
	this->place.set_x(0);
	this->place.set_y(0);
	this->width = 0;
	this->height = 0;
	this->text_color = 0;
	this->background_color = 250254;
}

Style::~Style() { }

void Style::set_attribute(std::string key, std::string value) {
    if (key == "left") {
        this->place.set_x(Tools::tonumber<int>(value));
    } else if (key == "top") {
        this->place.set_y(Tools::tonumber<int>(value));
    } else if (key == "width") {
        this->width = Tools::tonumber<int>(value);
    } else if (key == "height") {
        this->height = Tools::tonumber<int>(value);
    } else if (key == "color") {
        std::sscanf(value.c_str(), "rgb(%i, %i, %i);", &r, &g, &b);
        this->text_color = (r * 256 + g) * 256 + b;
    } else if (key == "background-color") {
        std::sscanf(value.c_str(), "rgb(%i, %i, %i);", &r, &g, &b);
        this->background_color = (r * 256 + g) * 256 + b;
    } else if (key == "class") {
        this->s_class = value;
    } else if (key == "name") {
		this->name = value;
	} else if (key == "id") {
		this->id = value;
	}
}

std::string Style::get_id() {
    return this->id;
}

std::string Style::get_class() {
    return this->s_class;
}

std::string Style::get_name() {
    return this->name;
}

Point& Style::get_place() {
	return this->place;
}

unsigned int Style::get_width() {
    return this->width;
}

unsigned int Style::get_height() {
    return this->height;
}

unsigned int Style::get_text_color() {
    return this->text_color;
}

unsigned int Style::get_background_color() {
    return this->background_color;
}

std::ostream& operator << (std::ostream& virta, const Style& style) {
	virta << style.place << " " << style.id << " " << style.s_class << " " << style.name << std::endl;
	virta << style.width << " " << style.height << " " << style.text_color << " " << style.background_color << std::endl;
	return virta;
}
