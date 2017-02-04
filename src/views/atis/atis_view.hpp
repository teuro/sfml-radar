#ifndef ATIS_VIEW
#define ATIS_VIEW

#include <vector>
#include <iostream>

#include "../view.hpp"
#include "../../tools/tools.hpp"
#include "../../tools/settings.hpp"
#include "../../models/atis.hpp"
#include "../../models/runway.hpp"

class Atisview : public View {
public:
	Atisview(Drawsurface& d, Settings& s, Atis*& a);
	~Atisview();
	void draw();
	void draw_runways(std::vector <Runway> runways);
    void load();
	std::string get_type(Point& mouse);
	std::string get_runway(std::vector <Runway> runways, Point& mouse);
	int get_altitude(Point& mouse);
	int get_level(Point& mouse);
private:
	Atis* atis;
	
	Point place_d;
	Point place_l;
	Point place_a;
	Point place_lv;
	int color;
};

#endif // ATIS_VIEW
