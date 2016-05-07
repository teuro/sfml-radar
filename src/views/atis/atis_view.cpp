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

void Atisview::draw_airfields(std::map <int, std::string>& airfields) {
	Point p1(250, 150);
	
	for (std::map<int, std::string>::iterator it = airfields.begin(); it != airfields.end(); ++it) {
		drawer.draw_text(it->second, p1, "red");
		p1.change_y(drawer.get_fontsize());
	}
}
