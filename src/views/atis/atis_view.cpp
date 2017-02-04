#include "atis_view.hpp"

Atisview::Atisview(Drawsurface& d, Settings& s, Atis*& a) : View(d, s), atis(a) { }

Atisview::~Atisview() { }

void Atisview::load(std::vector <Runway> runways) {
    View::load("atis");
	
	lists.clear();
	
	Drawable_list runway_list_dep("ul", "strorage", "runway-list-departure");
	Drawable_list runway_list_lnd("ul", "strorage", "runway-list-landing");
	Drawable_list transfer_level("ul", "strorage", "transfer-altitude");
	Drawable_list transfer_altitude("ul", "strorage", "transfer-level");
	
	for (unsigned int i = 0; i < runways.size(); ++i) {
		runway_list_dep.add_element(runways[i].get_name());
		runway_list_lnd.add_element(runways[i].get_name());
	}
	
	for (int i = -2; i < 3; ++i) {
		transfer_altitude.add_element(Tools::tostr(i * 1000 + 5000));
	}
	
	for (int i = 0; i < 7; ++i) {
		transfer_level.add_element(Tools::tostr(i * 5 + 35));
	}
	
	lists.push_back(runway_list_dep);
	lists.push_back(runway_list_lnd);
	lists.push_back(transfer_altitude);
	lists.push_back(transfer_level);
}

void Atisview::draw() {
    View::draw();
	
	for (unsigned int i = 0; i < this->lists.size(); ++i) {
		this->style(this->lists[i]);
		this->draw_element(this->lists[i]);
	}
}

std::string Atisview::get_value(Point& mouse) {
	std::list <std::string> t_list;
	std::list <std::string> :: iterator list_item;
	
	for (unsigned int i = 0; i < this->lists.size(); ++i) {
		t_list = this->lists[i].get_elements();
		list_item = t_list.begin();
		Point place = this->lists[i].get_style().get_place();
		
		while (list_item != t_list.end()) {
			if (Tools::on_area(mouse, place, 50, 10)) {
				std::string t = Tools::tostr(i) + "|" + (*list_item); 
				return t;
			}
			
			place.change_y(20);
			
			++list_item;
		}
	}
	
	return "";
}
