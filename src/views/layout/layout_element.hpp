#ifndef LAYOUT_ELEMENT_HPP
#define LAYOUT_ELEMENT_HPP

#include "../../models/point.hpp"
#include "../../tools/tools.hpp"

#include <list>
#include <string>
#include <vector>

class Layout_element {
public:
    Layout_element(std::string name, std::string id, std::string cl = "");
    Layout_element() { }
    ~Layout_element();
    Point& get_top_left();
    Point& get_bottom_right();
    std::string get_name();
    std::string get_id();

    void set_place(Point& p);
    void set_size(int width, int height);
    int get_height() { return (this->p2.get_y() - this->p1.get_y()); }
    void set_content(std::string content);

    bool operator ==(std::string id) { return (this->id == id); }
    std::string get_class() { return this->s_class; }
    std::vector <std::string> get_content();

    int t_red;
    int t_green;
    int t_blue;

    int b_red;
    int b_green;
    int b_blue;

    bool t_color_setted;
    bool b_color_setted;
private:
    std::string name;
    std::string id;
    std::string s_class;
    std::vector <std::string> v_content;

    Point p1;
    Point p2;
};

#endif // LAYOUT_ELEMENT_HPP
