#ifndef STAT_VIEW
#define STAT_VIEW

#include <vector>
#include <iostream>

#include "../view.hpp"
#include "../../tools/tools.hpp"
#include "../../tools/point_tools.hpp"
#include "../../tools/time_tools.hpp"
#include "../../tools/settings.hpp"
#include "../../models/game.hpp"

class Statview : public View {
public:
	Statview(Drawsurface& drawsurface, std::shared_ptr <Settings> settings, std::shared_ptr <Game> game);
	~Statview();
	virtual void draw(Point& mouse);
	virtual std::string handle_click(Point& mouse);
    void load();
	void draw_points();
	virtual void update();
private:
	std::shared_ptr <Game> game;
};

#endif // ATIS_VIEW
