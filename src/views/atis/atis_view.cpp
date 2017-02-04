#include "atis_view.hpp"

Atisview::Atisview(Drawsurface& d, Settings& s, Atis*& a) : View(d, s), atis(a) {
    this->load();
	this->place_d.set_place(50, 80);
	this->place_l.set_place(220, 80);
	this->place_a.set_place(400, 80);
	this->place_lv.set_place(500, 80);
	this->color = 152468;
}

Atisview::~Atisview() { }

void Atisview::load() {
    View::load("atis");
}

void Atisview::draw() {
    View::draw();
	this->drawer.draw_text("altitude", place_a, this->color);
	this->drawer.draw_text("level", place_lv, this->color);

	for (int i = -2; i < 3; ++i) {
		place_a.change_y(15);
		this->drawer.draw_text(Tools::tostr(i * 1000 + 5000), place_a, this->color);
	}
	
	for (int i = 0; i < 5; ++i) {
		place_lv.change_y(15);
		this->drawer.draw_text(Tools::tostr(i * 5 + 35), place_lv, this->color);
	}
		
	this->place_a.set_place(400, 80);
	this->place_lv.set_place(500, 80);
}

std::string Atisview::get_type(Point& mouse) {
	if (mouse.get_x() > this->place_d.get_x() && mouse.get_x() < this->place_d.get_x() + 50 && mouse.get_y() > this->place_d.get_y() && mouse.get_y() < this->place_d.get_y() + 550) {
		return "departure";
	} else if (mouse.get_x() > this->place_l.get_x() && mouse.get_x() < this->place_l.get_x() + 100 && mouse.get_y() > this->place_d.get_y() && mouse.get_y() < this->place_d.get_y() + 550) {
		return "landing";
	} else if (mouse.get_x() > this->place_a.get_x() && mouse.get_x() < this->place_a.get_x() + 100 && mouse.get_y() > this->place_a.get_y()-15 && mouse.get_y() < this->place_d.get_y() + 150) {
		return "altitude";
	} else if (mouse.get_x() > this->place_lv.get_x() && mouse.get_x() < this->place_lv.get_x() + 100 && mouse.get_y() > this->place_lv.get_y()-15 && mouse.get_y() < this->place_lv.get_y() + 150) {
		return "level";
	}
	
	return "";
}

std::string Atisview::get_runway(std::vector <Runway> runways, Point& mouse) {
	for (unsigned int i = 0; i < runways.size(); ++i) {
		this->place_d.change_y(15);
		
		if (mouse.get_x() > this->place_d.get_x() && mouse.get_x() < this->place_l.get_x() + 150 && mouse.get_y() > this->place_d.get_y() && mouse.get_y() < this->place_d.get_y() + 15) {
			return runways[i].get_name();
		}
	}
	
	return "";
}

int Atisview::get_altitude(Point& mouse) {
	this->place_a.change_y(15);
	
	for (int i = -2; i < 3; ++i) {
		this->place_a.change_y(15);
		
		if (mouse.get_x() > this->place_a.get_x() && mouse.get_x() < this->place_a.get_x() + 50 && mouse.get_y() > this->place_a.get_y()-15 && mouse.get_y() < this->place_a.get_y()) {
			return i * 1000 + 5000;
		}
	}
	
	return 0;
}

int Atisview::get_level(Point& mouse) {
	this->place_lv.change_y(15);
	
	for (unsigned int i = 0; i < 5; ++i) {
		this->place_lv.change_y(15);
		
		if (mouse.get_x() > this->place_lv.get_x() && mouse.get_x() < this->place_lv.get_x() + 50 && mouse.get_y() > this->place_lv.get_y()-15 && mouse.get_y() < this->place_lv.get_y()) {
			return i * 5 + 35;
		}
	}
	
	return 0;
}

void Atisview::draw_runways(std::vector <Runway> runways) {
	this->drawer.draw_text("departure", this->place_d, this->color);
	this->drawer.draw_text("landings", place_l, this->color);
	
	for (unsigned int i = 0; i < runways.size(); ++i) {
		place_d.change_y(15);
		place_l.change_y(15);

		this->drawer.draw_text(runways[i].get_name(), place_d, this->color);
		this->drawer.draw_text(runways[i].get_name(), place_l, this->color);
	}	
	
	this->place_d.set_place(50, 80);
	this->place_l.set_place(220, 80);
}
