#ifndef _VIEW_HPP
#define _VIEW_HPP

#include <iostream>
#include <tinyxml/tinyxml.h>
#include <tinyxpath/xpath_processor.h>

#include "../tools/drawing/sfml/sfml_drawsurface.hpp"
#include <map>
#include <list>
#include "layout/layout_element.hpp"
#include "layout/style.hpp"
#include "../tools/style/parse_css.hpp"

class View {
public:
	View(Drawsurface& d);
	~View();
	virtual void draw();
	void render();
	void clear_screen();
	void set_command(std::string command);
	std::string get_command();
protected:
    Drawsurface& drawer;
    TiXmlDocument document;
    std::string command;
    std::list <Style> styles;
    std::map <std::string, Layout_element> layout_elements;
    void draw_element(Layout_element& layout_element);
    void style(Layout_element& le);
    void load();
    Layout_element& get_element(std::string name);
    Point center_point;
};


#endif
