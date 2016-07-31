#include "style.hpp"

Style::Style(std::string i, int l, int t, int w, int h) : id(i), left(l), top(t), width(w), height(h), t_color(-1), b_color(-1) { }

Style::Style(std::string i) : id(i), t_color(-1), b_color(-1) { }

Style::~Style() { }

void Style::set_attribute(std::string key, std::string value) {
    if (key == "left") {
        this->left = Tools::tonumber<int>(value);
    } else if (key == "top") {
        this->top = Tools::tonumber<int>(value);
    } else if (key == "width") {
        this->width = Tools::tonumber<int>(value);
    } else if (key == "height") {
        this->height = Tools::tonumber<int>(value);
    } else if (key == "color") {
        std::sscanf(value.c_str(), "rgb(%i, %i, %i);", &r, &g, &b);
        this->t_color = (r * 256 + g) * 256 + b;
    } else if (key == "background-color") {
        std::sscanf(value.c_str(), "rgb(%i, %i, %i);", &r, &g, &b);
        this->b_color = (r * 256 + g) * 256 + b;
    } else if (key == "class") {
        this->s_class = value;
    }
}

std::string Style::get_id() {
    return this->id;
}

std::string Style::get_class() {
    return this->s_class;
}

unsigned int Style::get_left() {
    return this->left;
}

unsigned int Style::get_top() {
    return this->top;
}

unsigned int Style::get_width() {
    return this->width;
}

unsigned int Style::get_height() {
    return this->height;
}

int Style::get_t_color() {
    return this->t_color;
}

int Style::get_b_color() {
    return this->b_color;
}
