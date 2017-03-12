#include "style.hpp"

Style::Style(Settings& s) : settings(s) { 
	this->place.set_x(0);
	this->place.set_y(0);
	this->left = 0;
	this->top = 0;
	this->width = 0;
	this->height = 0;
	this->text_color = 0;
	this->background_color = 0;
}

Style::Style() { 
	this->place.set_x(0);
	this->place.set_y(0);
	this->left = 0;
	this->top = 0;
	this->width = 0;
	this->height = 0;
	this->text_color = 0;
	this->background_color = 0;
}
Style::~Style() { }

void Style::set_attribute(std::string key, int value) {
	//std::clog << "Style::set_attribute(" << key << ", " << value << ")" << std::endl;
	if (value > 0) {
		if (key == "color") {
			this->text_color = value;
		} else if (key == "background-color") {
			this->background_color = value;
		} else if (key == "border") {
			this->border_color = value;
		} else { 
			this->set_attribute(key, Tools::tostr(value));
		}
	}
}

void Style::set_attribute(std::string key, std::string value) {
	//std::clog << "Style::set_attribute(" << key << ", " << value << ")" << std::endl;
	
	if (value.length() == 0) {
		return;
	}
	
    if (key == "left") {
        this->place.set_x(Tools::tonumber<int>(value));
		this->left = Tools::tonumber<int>(value);
    } else if (key == "right") {
        this->place.set_x(this->settings.screen_width - Tools::tonumber<int>(value));
		this->left = this->settings.screen_width - Tools::tonumber<int>(value);
    } else if (key == "top") {
        this->place.set_y(Tools::tonumber<int>(value));
		this->top = Tools::tonumber<int>(value);
    } else if (key == "bottom") {
        this->place.set_y(this->settings.screen_height - Tools::tonumber<int>(value));
		this->top = this->settings.screen_height - Tools::tonumber<int>(value);
    }else if (key == "width") {
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
	} else if (key == "shape") {
		this->shape = value;
	} else if (key == "position") {
		this->position = value;
	} else if (key == "border") {
		std::sscanf(value.c_str(), "rgb(%i, %i, %i);", &r, &g, &b);
        this->border_color = (r * 256 + g) * 256 + b;
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

std::string Style::get_shape() {
    return this->shape;
}

std::string Style::get_position() {
    return this->position;
}

Point& Style::get_place() {
	return this->place;
}

unsigned int Style::get_left() {
    return this->place.get_x();
}

unsigned int Style::get_top() {
    return this->place.get_y();
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

unsigned int Style::get_border_color() {
    return this->border_color;
}

void Style::set_place(Point& t_place) {
	this->place = t_place;
}

std::ostream& operator << (std::ostream& virta, const Style& style) {
	virta << "place = " << style.place << " id = " << style.id << " class = " << style.s_class << " name = " << style.name << std::endl;
	virta << "width = " << style.width << " height = " << style.height << " text-color = " << style.text_color << " background-color = " << style.background_color << std::endl;
	virta << "position = " << style.position << " left = " << style.left << " top = " << style.top << " shape = " << style.shape << std::endl;
	return virta;
}
