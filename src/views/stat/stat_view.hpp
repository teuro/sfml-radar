#ifndef STAT_VIEW
#define STAT_VIEW

#include <vector>
#include <iostream>

#include "../view.hpp"
#include "../../tools/tools.hpp"
#include "../../tools/settings.hpp"
#include "../../models/game.hpp"

class Statview : public View {
public:
	Statview(Drawsurface& d, Settings& s);
	~Statview();
	void draw();
    void load();
	void draw_clearances(std::list <Clearance> clearances);
};

#endif // ATIS_VIEW
