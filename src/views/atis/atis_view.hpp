#ifndef ATIS_VIEW
#define ATIS_VIEW

#include <vector>
#include <iostream>

#include "../view.hpp"
#include "../../tools/tools.hpp"
#include "../../tools/point_tools.hpp"
#include "../../tools/settings.hpp"
#include "../../models/weather/atis.hpp"
#include "../../models/game_objects/runway.hpp"

class Atisview : public View {
public:
	Atisview(Drawsurface& d, Settings& s, Atis*& a);
	~Atisview();
	void draw();
    void load();
	std::string get_value(Point& mouse);
	void draw_errors();
	void update();
private:
	Atis* atis;
};

#endif // ATIS_VIEW
