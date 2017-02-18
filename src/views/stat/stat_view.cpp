#include "stat_view.hpp"
Statview::Statview(Drawsurface& d, Settings& s) : View(d, s) { }

Statview::~Statview() { }

void Statview::draw() { 
	 View::draw();
}

void Statview::load() { 
	View::load("stat");
}

void Statview::draw_clearances(std::list <Clearance> clearances) {
	std::list <Clearance> :: iterator cit = clearances.begin();
	
	Point place(120, 50);
	int color = 1524875;
	
	while (cit != clearances.end()) {
		this->drawer.draw_text(Tools::totime(cit->time, "H:i:s"), place, color);
		place.change_x(100);
		this->drawer.draw_text(cit->plane, place, color);
		place.change_x(100);
		this->drawer.draw_text(cit->clearance, place, color);
		place.change_y(20);
		place.change_x(-200);
		++cit;
	}
}