#include "menu_view.hpp"

Menuview::Menuview(Drawsurface& d, Settings& s, Menu*& m) : View(d, s), menu(m) { }

Menuview::~Menuview() { }

void Menuview::draw() { 
	View::draw();
	
	Drawable_list d_list("ul", "airfield_list", "airfield_list");
	
	std::list <std::string> t_air = this->menu->get_airports();
	
	std::list <std::string> :: iterator it = t_air.begin();
	
	for (it = t_air.begin(); it != t_air.end(); ++it) {
		d_list.add_element(*it);
	}
	
	this->style(d_list);
	this->lists.push_back(d_list);
}

void Menuview::load() { 
	View::load("menu");
}
