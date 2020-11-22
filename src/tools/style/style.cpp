#include "style.hpp"

Style::Style(std::shared_ptr <Settings> s) : settings(s) { 
	this->place.set_x(0);
	this->place.set_y(0);
	this->left = 0;
	this->top = 0;
	this->width = 0;
	this->height = 0;
	this->text_color = 0;
	this->background_color = 0;
	this->border_color = 0;
	this->margin = 5;
	this->font_size = 16;
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
	this->border_color = 0;
	this->margin = 5;
	this->font_size = 16;
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
		} else if (key == "margin") {
			this->margin = value;
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
        this->place.set_x(this->settings->screen_width - Tools::tonumber<int>(value));
		this->left = this->settings->screen_width - Tools::tonumber<int>(value);
    } else if (key == "top") {
        this->place.set_y(Tools::tonumber<int>(value));
		this->top = Tools::tonumber<int>(value);
    } else if (key == "bottom") {
        this->place.set_y(this->settings->screen_height - Tools::tonumber<int>(value));
		this->top = this->settings->screen_height - Tools::tonumber<int>(value);
    }else if (key == "width") {
        this->width = Tools::tonumber<int>(value);
    } else if (key == "height") {
        this->height = Tools::tonumber<int>(value);
    } else if (key == "color") {
        this->text_color = this->parse_color(value);
    } else if (key == "background-color") {
        this->background_color = this->parse_color(value);
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
	} else if (key == "margin") {
		this->margin = Tools::toint(value);
	} else if (key == "border") {
		this->border_color = this->parse_color(value);
	} else if (key == "font") {
		this->font_size = Tools::toint(value);
	}
}

unsigned int Style::parse_color(std::string color) {
	int red, green, blue;
	
	if (std::sscanf(color.c_str(), "rgb(%i, %i, %i);", &red, &green, &blue) == 3) {
		
	} else if (color.substr(0, 1 ) == "#") {
		color = color.substr(1);
		
		std::size_t found = color.find(";");

		if (found != std::string::npos) {
			color = color.substr(0, found);
		}
		
		int t_color = Tools::hex2int(color);
		
		red =   ( t_color >>  0 ) & 255;
		green = ( t_color >>  8 ) & 255;
		blue =  ( t_color >> 16 ) & 255;
	}
	
	My_Color t_color(red, green, blue);
	
	return t_color.get_color();
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

unsigned int Style::get_border_thickness() {
    return this->border_thickness;
}

unsigned int Style::get_margin() {
    return this->margin;
}

unsigned int Style::get_font_size() {
    return this->font_size;
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
