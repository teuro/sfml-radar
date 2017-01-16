#ifndef STYLE_HPP
#define STYLE_HPP

#include <string>
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include "../tools.hpp"
#include "../../models/point.hpp"

class Style {
public:
    Style();
    ~Style();

    std::string get_id();
    std::string get_class();
    std::string get_name();
	
	Point& get_place();
	
    unsigned int get_width();
    unsigned int get_height();
    unsigned int get_text_color();
    unsigned int get_background_color();
	
    void set_attribute(std::string key, std::string value);
	
	friend std::ostream& operator << (std::ostream& virta, const Style& style);
protected:
private:
    Point place; /* top left corner */
    std::string id;
    std::string s_class;
	std::string name;
    int width;
    int height;
    int text_color;
    int background_color;
	
	int r, g, b; /* pure help variables */
};

#endif // STYLE_HPP
