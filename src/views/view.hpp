#ifndef _VIEW_HPP
#define _VIEW_HPP

#include <iostream>
#include <tinyxml/tinyxml.h>

#include "../tools/drawing/sfml/sfml_drawsurface.hpp"
#include <map>
#include <vector>
#include <list>
#include <fstream>
#include "layout/drawable_image.hpp"
#include "layout/drawable_paragraph.hpp"
#include "layout/drawable_list.hpp"
#include "layout/drawable_table.hpp"
#include "layout/drawable_input.hpp"
#include "../tools/style/parse_css.hpp"
#include "../tools/style/style.hpp"
#include "../tools/settings.hpp"

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
	void draw_element(Drawable_list& dl);
	void draw_element(Drawable_table& dt);

	void style(Drawable_element& de);
    void load(std::string state);
    Point centerpoint_screen;
	std::vector <Image> images;
	std::vector <Paragraph> paragraphs;
	std::vector <Drawable_input> inputs;
	std::vector <Drawable_list> lists;
	std::vector <Drawable_table> tables;
private:
};

#endif
