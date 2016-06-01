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
	for (unsigned int i = 0; i < depa.size(); ++i) {
		if (Tools::on_area(mouse, depa[i].place_lu, depa[i].place_rd)) {
			this->selected_dep = i;
			return depa[i].name;
		}
	}
	
	return "";
}

std::string Atisview::get_lnd_runway_name(Point& mouse) {
	for (unsigned int i = 0; i < land.size(); ++i) {
		if (Tools::on_area(mouse, land[i].place_lu, land[i].place_rd)) {
			this->selected_lnd = i;
			return land[i].name;
		}
	}
	
	return "";
}

void Atisview::draw_runways(std::list <std::string>& runways) {
	this->calculate_places(runways);
	std::string color;
	
	for (unsigned int i = 0; i < depa.size(); ++i) {
		color = (i == this->selected_dep) ? "green" : "red";
		drawer.draw_text(depa[i].name, depa[i].place_lu, color);
	}
	
	for (unsigned int i = 0; i < land.size(); ++i) {
		color = (i == this->selected_lnd) ? "green" : "red";
		drawer.draw_text(land[i].name, land[i].place_lu, color);
	}
}

void Atisview::calculate_places(std::list <std::string>& runways) {
	this->depa.clear();
	this->land.clear();
	
	Point p1(250, 150);
	Point p2(450, 150);
	
	int width = 60;
	int height = drawer.get_fontsize();
	
	drawer.draw_text("departure", p1, "red");
	drawer.draw_text("landing", p2, "red");
	
	for (std::list<std::string>::iterator it = runways.begin(); it != runways.end(); ++it) {
		p1.change_y(drawer.get_fontsize()+10);
		p2.change_y(drawer.get_fontsize()+10);
		
		Point p1rd(p1.get_x() + width, p1.get_y() + height);
		Point p2rd(p2.get_x() + width, p2.get_y() + height);
		
		depa.push_back(Rwy(*it, p1, p1rd));
		land.push_back(Rwy(*it, p2, p2rd));
	}
}
