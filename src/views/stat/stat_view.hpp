#ifndef STAT_VIEW
#define STAT_VIEW

#include <vector>
#include <iostream>

#include "../view.hpp"
#include "../../tools/tools.hpp"
#include "../../tools/point_tools.hpp"
#include "../../tools/settings.hpp"
#include "../../models/game.hpp"

class Statview : public View {
public:
	Statview(Drawsurface& d, Settings& s);
	~Statview();
	void draw();
    void load();
	void draw_points(std::list <Game_point> points);
};

#endif // ATIS_VIEW
