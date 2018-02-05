#ifndef ATIS_VIEW
#define ATIS_VIEW

#include <vector>
#include <iostream>

#include "../view.hpp"
#include "../menu/menu_view.hpp"
#include "../../tools/tools.hpp"
#include "../../tools/point_tools.hpp"
#include "../../tools/settings.hpp"
#include "../../models/weather/atis.hpp"
#include "../../models/game_objects/runway.hpp"

class Atisview : public View {
public:
	Atisview(Drawsurface& d, std::shared_ptr <Settings> s, std::shared_ptr <Atis> a);
	~Atisview();
	virtual void draw(Point& mouse);
	virtual std::string handle_click(Point& mouse);
    void load();
	std::string get_value();
	void draw_errors();
	virtual void update();
private:
	std::shared_ptr <Atis> atis;
	std::shared_ptr <Menuview> menuview;
	
	void draw_runways();
};

#endif // ATIS_VIEW
