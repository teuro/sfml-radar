#ifndef _VIEW_HPP
#define _VIEW_HPP

#include <iostream>
#include <tinyxml/tinyxml.h>

#include "../tools/drawing/sfml/sfml_drawsurface.hpp"
#include <dirent.h>
#include <map>
#include <vector>
#include <list>
#include <set>
#include <fstream>
#include <memory>

#include "layout/drawable_text_element.hpp"
#include "layout/drawable_image.hpp"
#include "layout/drawable_paragraph.hpp"
#include "layout/drawable_list.hpp"
#include "layout/drawable_table.hpp"
#include "layout/drawable_input.hpp"
#include "layout/drawable_Runway_Element.hpp"
#include "layout/drawable_navpoint_element.hpp"
#include "layout/clickable.hpp"
#include "../tools/style/style.hpp"
#include "../tools/style_tools.hpp"
#include "../tools/settings.hpp"
#include <tools/tools.hpp>
#include <tools/text_tools.hpp>
#include "../models/menu/menu.hpp"

/**
	* View
	* This is base class for all views. This parses css and handles all styling. 
	* This helps others views because this offers many helper methods such as draw_element.
	* Other views may on call appropriate methods without caring how its implemented.
**/

class View {
protected:
	/**
		* View constructor
		* @param Drawsurface
		* @param std::shared_ptr settings
		* @return void
	**/
	
	View(Drawsurface& d, std::shared_ptr <Settings> s);
	
	/**
		* 	Destructor this is pure virtual and protected because You can't make View 
			It's allways require proper child class.
	**/
	virtual ~View() = 0;
public:
	/**
		* Draw
		* Virtual method to draw object
		* @param Point& mouse
		* @return void
	**/
	
	virtual void draw(Point& mouse) = 0;
	void draw();
	virtual std::string handle_click(Point& mouse) = 0;
	
	/**
		* render
		* This flips the screen visible
	**/
	
	void render();
	
	/**
		* clear_screen
		* Fills screen with proper background-color. If background-color is not defined it's black
	**/
	
	void clear_screen();
	
	virtual void update() = 0;
	
	/**
		* Draw menu
		* Designed for draw menu
		* @param std::shared_ptr <Menu>
		* @return void
	**/
	
	void draw_menu(std::shared_ptr <Menu> menu);
	
	int handled;
	int required;
	std::string metar;
	std::map <std::string, std::string> repl;
	void flash_message(std::string message);
	virtual void load() = 0;
	virtual double distanceNM(double pixels);
	virtual void set_menu(std::shared_ptr <Menu> menu);
	virtual void update_command(std::string command);
protected:
	double min_lat;
	double max_lat;
	double min_lon;
	double max_lon;

    Drawsurface& drawer;
    std::shared_ptr <Settings> settings;
    std::shared_ptr <Menu> menu;
    std::list <Style> styles;
	
    void draw_element(Image& img);
    void draw_element(Paragraph& p);
    void draw_element(std::string text, Point& place, unsigned int color, int font_size);
    void draw_element(std::shared_ptr <Drawable_input>& di);
	void draw_element(Drawable_list& dl);
	void draw_element(Drawable_table& dt);
	void draw_borders(Style& style);
	void draw_element(Drawable_Runway_Element& re);
	void draw_element(Drawable_Navpoint_Element& re);
	virtual void calculate_coordinate_limits();
	
	void style(Drawable_element& de);
	void style(std::shared_ptr <Drawable_input>& de);
    void load(std::string state);
    Point centerpoint_screen;
	std::vector <Image> images;
	std::vector <Paragraph> paragraphs;
	std::vector <Drawable_list> lists;
	std::vector <Drawable_table> tables;
	std::vector <Clickable> clicks;
	std::list <std::string> style_files;
	std::string command;
private:
	void add_style(std::list <Style> tmp);
	void load_styles();
	void load_layout(std::string state);
	Style find_style(std::string name, std::set <std::string> t_classes, std::string id);
	Style find_style(std::string name); 
	Style body;
};

#endif
