#include "layout_element.hpp"

Layout_element::Layout_element(std::string n, int top, int left, int width, int height) : name(n){
    p1.set_place(left, top);
    p2.set_place(left+width, top+height);
}

Layout_element::~Layout_element() { }

Point& Layout_element::get_top_left() {
    return p1;
}

Point& Layout_element::get_bottom_right() {
    return p2;
}

std::string Layout_element::get_name() {
    return this->name;
}
