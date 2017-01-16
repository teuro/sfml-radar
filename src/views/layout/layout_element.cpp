#include "layout_element.hpp"

Layout_element::Layout_element(std::string n, std::string i, std::string cl) : name(n), id(i), s_class(cl) {
    this->t_color_setted = false;
    this->t_color_setted = false;
    this->p1.set_place(10, 10);
    this->p2.set_place(60, 60);
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

std::string Layout_element::get_class() {
    return this->s_class;
}

void Layout_element::set_place(Point& p) {
    this->p1 = p;
}

void Layout_element::set_size(int width, int height) {
    //std::clog << this->name << " new size is " << width << ", " << height << std::endl;
    this->p2.set_x(this->p1.get_x() + width);
    this->p2.set_y(this->p1.get_y() + height);
}

void Layout_element::set_content(std::string content) {
	this->v_content.clear();
	this->v_content.push_back(content);
}

void Layout_element::add_content(std::string content) {
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

void Layout_element::set_text_colors(int color) {
	this->t_blue = color % 256;
	color /= 256;
	this->t_red = color / 256;
	this->t_green = color % 256;

	this->t_color_setted = true;
}

void Layout_element::set_background_colors(int color) {
	this->b_blue = color % 256;
	color /= 256;
	this->b_red = color / 256;
	this->b_green = color % 256;

	this->b_color_setted = true;
}

void Layout_element::set_id(std::string tid) {
	this->id = tid;
}

std::string Layout_element::get_color() {
	return Tools::tostr(this->t_red) + ", " + Tools::tostr(this->t_green) + ", " + Tools::tostr(this->t_blue);
}