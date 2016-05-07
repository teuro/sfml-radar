#include "atis_view.hpp"

Atisview::Atisview(Drawsurface& d, Settings& s) : View(d), settings(s) {
    this->load();
}

Atisview::~Atisview() { }

void Atisview::load() {
    View::load();
}

void Atisview::draw() {
    View::draw();
}

void Atisview::draw_runways(std::list <std::string>& runways) {
	Point p1(250, 150);
	Point p2(450, 150);
	
	drawer.draw_text("departure", p1, "red");
	drawer.draw_text("landing", p2, "red");
	
	for (std::list<std::string>::iterator it = runways.begin(); it != runways.end(); ++it) {
		p1.change_y(drawer.get_fontsize());
		p2.change_y(drawer.get_fontsize());
		
		drawer.draw_text(*it, p1, "red");
		drawer.draw_text(*it, p2, "red");
	}
}
