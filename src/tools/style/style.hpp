#ifndef STYLE_HPP
#define STYLE_HPP

#include <string>
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include "../tools.hpp"

class Style {
public:
    Style(std::string i, int l, int t, int w, int h);
    Style(std::string i);
    ~Style();

    std::string get_id();
    std::string get_class();
    unsigned int get_left();
    unsigned int get_top();
    unsigned int get_width();
    unsigned int get_height();

    void set_width(int width) {this->width = width; }
    void set_height(int height) {this->height = height; }

    void set_attribute(std::string key, std::string value);
    int get_t_color();
    int get_b_color();
protected:
private:
    std::string id;
    std::string s_class;
    int left;
    int top;
    int width;
    int height;
    int t_color;
    int b_color;
    int r, g, b;
};

#endif // STYLE_HPP
