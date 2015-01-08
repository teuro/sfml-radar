#ifndef _GAME_VIEW_HPP
#define _GAME_VIEW_HPP

#include <vector>
#include <list>
#include <map>
#include <iostream>
#include <tinyxml/tinyxml.h>
#include <tinyxpath/xpath_processor.h>

#include "../view.hpp"
#include "../../tools/tools.hpp"
#include "../../tools/settings.hpp"
#include "../../models/coordinate.hpp"
#include "../../models/navpoint.hpp"
#include "../../models/aircraft.hpp"
#include "../../models/airfield.hpp"
#include "../layout/layout_element.hpp"
#include "../layout/style.hpp"
#include "../../tools/style/parse_css.hpp"

class Gameview : public View {
public:
	Gameview(Drawsurface& d, Settings& s);
	~Gameview();
	void draw();
	Layout_element& get_element(std::string name);
	void draw_navpoints(std::vector <Navpoint>& navpoints);
	void draw_airfield(Airfield* airfield);
	void draw_planes(std::list <Aircraft*> planes, Aircraft* selected);
	void set_reply(std::string reply);
	void render();
	void clear_screen();
	void set_command(std::string command);
	void update_command(std::string command);
	std::string get_command();
private:
    Settings& settings;

    void draw_plane(Aircraft*& plane, Point& center_point, std::string color);
    void draw_element(Layout_element& layout_element);
    void style(Layout_element& le);
    void read_data(std::string datafile);
    void set_styles(std::map <std::string, Layout_element>& layout_elements);
    std::map <std::string, Layout_element> layout_elements;
    void load();
    TiXmlDocument document;
    Point center_point;
    std::list <Style> styles;
    std::string command;
    std::string callsign;
    std::string reply;
};

#endif // _GAME_VIEW_HPP
