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
	Atisview(Drawsurface& d, Settings& s, std::shared_ptr <Atis> a);
	~Atisview();
	void draw();
    void load();
	std::string get_value(Point& mouse);
	void draw_errors();
	void update();
private:
	std::shared_ptr <Atis> atis;
	
	void draw_runways();
};

#endif // ATIS_VIEW
