#ifndef ATIS_VIEW
#define ATIS_VIEW

#include <vector>
#include <list>
#include <iostream>

#include "../view.hpp"
#include "../../tools/tools.hpp"
#include "../../tools/settings.hpp"
#include "../../models/atis.hpp"

class Rwy {
public:
	Rwy(std::string n, Point p1, Point p2);
	std::string name;
	Point place_lu;
	Point place_rd;
};

class Atisview : public View {
public:
	Atisview(Drawsurface& d, Settings& s);
	~Atisview();
	void draw();
	void draw_runways(std::list <std::string>& runways);
	void calculate_places(std::list <std::string>& runways);
	std::string get_dep_runway_name(Point& mouse);
	std::string get_lnd_runway_name(Point& mouse);
private:
    Settings& settings;
    void load();
	
	std::vector <Rwy> land;
	std::vector <Rwy> depa;
	
	unsigned int selected_dep;
	unsigned int selected_lnd;
};

#endif // ATIS_VIEW
