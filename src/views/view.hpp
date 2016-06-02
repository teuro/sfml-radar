#ifndef _VIEW_HPP
#define _VIEW_HPP

#include <iostream>
#include <tinyxml/tinyxml.h>

#include "../tools/drawing/sfml/sfml_drawsurface.hpp"
#include <map>
#include <vector>
#include <list>
#include "layout/layout_element.hpp"
#include "layout/style.hpp"
#include "../tools/style/parse_css.hpp"
#include "../tools/settings.hpp"

struct Image {
	std::string source;
	std::string id;
	Point place;
};

struct Paragraph {
	std::string content;
	std::string id;
	Point place;
};

class View {
public:
	View(Drawsurface& d, Settings& s);
	virtual ~View() = 0;
	virtual void draw();
	void render();
	void clear_screen();
	void iterate(TiXmlNode* el);
	void add_element(std::string key, std::string id, std::string cl, std::string value);
	int handled;
	int required;
	std::string metar;
	std::map <std::string, std::string> repl;
protected:
    Drawsurface& drawer;
    Settings& settings;
    std::list <Style> styles;
    std::map <std::string, Layout_element> layout_elements;
    void draw_element(Layout_element& layout_element);
    void draw_element(Image& img);
    void draw_element(Paragraph& p);
    void style(Layout_element& le);
	void style(Image& img);
	void style(Paragraph& p);
	
	/**
		@todo load all element from xml 
	**/
    void load();
    Layout_element& get_element(std::string name);
    Point center_point;
	std::vector <Image> images;
	std::vector <Paragraph> paragraphs;
private:
	
};


#endif
