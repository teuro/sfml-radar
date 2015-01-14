#include "layout_element.hpp"

Layout_element::Layout_element(std::string n, std::string i, std::string cl) : name(n), id(i), s_class(cl) {
    this->t_color_setted = false;
    this->t_color_setted = false;
}

Layout_element::~Layout_element() {
    this->t_color_setted = false;
    this->b_color_setted = false;
}

Point& Layout_element::get_top_left() {
    return p1;
}

Point& Layout_element::get_bottom_right() {
    return p2;
}

std::vector <std::string> Layout_element::get_content() {
    return this->v_content;
}

std::string Layout_element::get_name() {
    return this->name;
}

std::string Layout_element::get_id() {
    return this->id;
}

void Layout_element::set_place(Point& p) {
    //std::clog << this->name << " place is " << p.get_x() << ", " << p.get_y() << std::endl;
    this->p1 = p;
}

void Layout_element::set_size(int width, int height) {
    //std::clog << this->name << " new size is " << width << ", " << height << std::endl;
    this->p2.set_x(this->p1.get_x() + width);
    this->p2.set_y(this->p1.get_y() + height);
}

void Layout_element::set_content(std::string content) {
    unsigned int pos = content.find(";");

    if (pos != std::string::npos) {
        this->v_content = Tools::split(";", content);
        this->v_content.erase(this->v_content.end()-1);
    } else {
        this->v_content.push_back(content);
    }
}

void Layout_element::update_content(std::string content) {
    this->set_content(content);
}
