#include "atis_view.hpp"

Rwy::Rwy(std::string n, Point p1, Point p2) : name(n), place_lu(p1), place_rd(p2) { }

Atisview::Atisview(Drawsurface& d, Settings& s) : View(d, s) {
    this->load();
}

Atisview::~Atisview() { }

void Atisview::load() {
    View::load();
}

void Atisview::draw() {
    View::draw();
}

std::string Atisview::get_dep_runway_name(Point& mouse) {
	
	return "";
}

std::string Atisview::get_lnd_runway_name(Point& mouse) {
	return "";
}

void Atisview::draw_runways(std::list <std::string>& runways) {
	this->calculate_places(runways);
}

void Atisview::calculate_places(std::list <std::string>& runways) {
	
}

void Atisview::style(Drawable_element& de) {

}
