#ifndef _GAME_VIEW_HPP
#define _GAME_VIEW_HPP

#include <vector>
#include <list>
#include <iostream>
#include <tinyxml/tinyxml.h>
#include <tinyxpath/xpath_processor.h>

#include "view.hpp"
#include "../models/game.hpp"
#include "../tools/tools.hpp"
#include "../tools/settings.hpp"
#include "../models/coordinate.hpp"
#include "../models/aircraft.hpp"
#include "layout_element.hpp"

class Gameview : public View {
public:
	Gameview(Game& g, Drawsurface& d, Settings& s);
	~Gameview();
	void draw();
	Layout_element& get_element(std::string name);
private:
    Game& game;
    Settings& settings;

    void draw_plane(Aircraft*& plane, Point& center_point, std::string color);
    void draw_plane(std::string callsign, double latitude, double longitude, double speed, double altitude, double heading, Point& center_point, std::string color);
    void draw_clearance(Aircraft*& plane, Point& place, std::string color);
    void draw_element(Layout_element& layout_element);
    std::vector <Layout_element> layout_elements;
    void load();
    TiXmlDocument document;
    Point center_point;
};

#endif // _GAME_VIEW_HPP
