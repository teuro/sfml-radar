#ifndef LAYOUT_ELEMENT_HPP
#define LAYOUT_ELEMENT_HPP

#include "../models/point.hpp"

#include <list>
#include <string>

class Layout_element {
public:
    Layout_element(std::string n, int top, int left, int width, int height);
    ~Layout_element();
    Point& get_top_left();
    Point& get_bottom_right();
    std::string get_name();
private:
    std::string name;

    Point p1;
    Point p2;
};

#endif // LAYOUT_ELEMENT_HPP
