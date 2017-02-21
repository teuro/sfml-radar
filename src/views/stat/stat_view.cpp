#include "stat_view.hpp"
Statview::Statview(Drawsurface& d, Settings& s) : View(d, s) { }

Statview::~Statview() { }

void Statview::draw() { 
	 View::draw();
}

void Statview::load() { 
	std::clog << "Statview::load()" << std::endl;
	View::load("stat");
}

void Statview::draw_points(std::list <Game_point> points) {
	std::list <Game_point> :: iterator pit = points.begin();
	
	Point place(120, 50);
	int color = 1524875;
	
	while (pit != points.end()) {
		this->drawer.draw_text(pit->plane, place, color);
		place.change_x(100);
		this->drawer.draw_text(Tools::tostr(pit->points, 3), place, color);
		place.change_y(20);
		place.change_x(-100);
		++pit;
	}
}