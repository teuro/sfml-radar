#ifndef ATIS_VIEW
#define ATIS_VIEW

#include <vector>
#include <list>
#include <iostream>

#include "../view.hpp"
#include "../../tools/tools.hpp"
#include "../../tools/settings.hpp"
#include "../../models/atis.hpp"

/**
	* @todo write atisview 
**/

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
    void load();
};

#endif // ATIS_VIEW
