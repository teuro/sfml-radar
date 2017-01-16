#ifndef _VIEW_HPP
#define _VIEW_HPP

#include <iostream>
#include <tinyxml/tinyxml.h>

#include "../tools/drawing/sfml/sfml_drawsurface.hpp"
#include <map>
#include <vector>
#include <list>
#include <fstream>
#include "layout/layout_element.hpp"
#include "layout/drawable_element.hpp"
#include "../tools/style/parse_css.hpp"
#include "../tools/style/style.hpp"
#include "../tools/settings.hpp"

class Image : public Drawable_element {
public:
	Image(std::string source, std::string t_name, std::string t_class, std::string t_id);
	
	std::string get_source();
	void set_source(std::string src);
private:
	std::string source;
};

class Paragraph : public Drawable_element {
public:
	Paragraph(std::string content, std::string t_name, std::string t_class, std::string t_id);
	
	std::string get_content();
	void set_content(std::string cnt);
private:
	std::string content;
};

class Input : public Drawable_element {
public:
	Input(std::string t_name, std::string t_class, std::string t_id);
	void set_content(std::string cnt);
	std::string get_content();
private:
	std::string content;
};

class View {
public:
	View(Drawsurface& d, Settings& s);
	virtual ~View() = 0;
	void draw();
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
    void load();
    Layout_element& get_element(std::string name);
    Point centerpoint_screen;
	std::vector <Image> images;
	std::vector <Paragraph> paragraphs;
	std::vector <Input> inputs;
private:
};


#endif
