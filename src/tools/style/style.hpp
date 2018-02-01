#ifndef STYLE_HPP
#define STYLE_HPP

/**
	* This part of program reads and parses well formed *.css files.
**/

#include <string>
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <list>
#include "../tools.hpp"
#include "../text_tools.hpp"
#include "../settings.hpp"
#include "../../models/coordinate/point.hpp"
#include "../drawing/mycolor.hpp"

class Style {
public:
    Style();
    Style(std::shared_ptr <Settings> s);
    ~Style();

    std::string get_id();
    std::string get_class();
    std::string get_name();
    std::string get_shape();
    std::string get_position();
	
	Point& get_place();
	
    unsigned int get_left();
    unsigned int get_top();
    unsigned int get_width();
    unsigned int get_height();
    unsigned int get_text_color();
    unsigned int get_background_color();
    unsigned int get_border_color();
    unsigned int get_margin();
	
    void set_attribute(std::string key, std::string value);
    void set_attribute(std::string key, int value);
	
	void set_place(Point& place);
	
	friend std::ostream& operator << (std::ostream& virta, const Style& style);
	
	bool operator ==(std::string name) {
		return this->name == name;
	}
protected:
private:
    Point place; /* top left corner */
    std::string id;
    std::string s_class;
	std::string name;
	std::string position;
	std::string shape;
	std::string border;
	
    int width;
    int height;
    int text_color;
    int background_color;
    int border_color;
	int left;
	int top;
	int margin;
	
	std::shared_ptr <Settings> settings;
	
	template <class T>
	
	int parse_color(int color);
	int parse_color(std::string color);
};

#endif // STYLE_HPP
