#ifndef _GAME_VIEW_HPP
#define _GAME_VIEW_HPP

#include <vector>
#include <list>
#include <map>
#include <iostream>

#include "../view.hpp"
#include "../../tools/tools.hpp"
#include "../../tools/settings.hpp"
#include "../../models/coordinate.hpp"
#include "../../models/navpoint.hpp"
#include "../../models/aircraft.hpp"
#include "../../models/airfield.hpp"

/**
	* Gameview is responsible to render game to screen.
	* view 
**/

class Gameview : public View {
public:
	Gameview(Drawsurface& d, Settings& s);
	~Gameview();
	void draw();
	void load();

	void draw_navpoints(std::vector <Navpoint>& navpoints);
	void draw_airfield(Airfield* airfield);
	void draw_planes(std::list <Aircraft*> planes, Aircraft* selected);
	void set_centerpoint(Coordinate& cp);
private:
    Settings& settings;

    void draw_plane(Aircraft*& plane, std::string color);
    void read_data(std::string datafile);
	Coordinate centerpoint;
};

#endif // _GAME_VIEW_HPP
