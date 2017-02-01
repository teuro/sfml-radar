#ifndef _VIEW_HPP
#define _VIEW_HPP

#include <iostream>
#include <tinyxml/tinyxml.h>

#include "../tools/drawing/sfml/sfml_drawsurface.hpp"
#include <map>
#include <vector>
#include <list>
#include <fstream>
#include "layout/drawable_element.hpp"
#include "../tools/style/parse_css.hpp"
#include "../tools/style/style.hpp"
#include "../tools/settings.hpp"

class Drawable_input : public Drawable_element {
public:
	Drawable_input(std::string value, std::string t_name, std::string t_class, std::string t_id);
	void set_value(std::string value);
	std::string get_value();
private:
	std::string value;
};

class Drawable_list : public Drawable_element {
public:
	Drawable_list(std::string t_name, std::string t_class, std::string t_id);
	void add_element(std::string content);
	std::list <std::string> get_elements();
	void clear_content();
private:
	std::list <std::string> elements;
};

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
    void draw_element(Image& img);
    void draw_element(Paragraph& p);
    void draw_element(Drawable_element& de);
    void draw_element(Drawable_input& di);

	void style(Drawable_element& de);
    void load();
    Point centerpoint_screen;
	std::vector <Image> images;
	std::vector <Paragraph> paragraphs;
	std::vector <Drawable_input> inputs;
private:
};


#endif
