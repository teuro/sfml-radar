#include "menu_view.hpp"

Menuview::Menuview(Drawsurface& d, Settings& s, Menu*& m) : View(d, s), menu(m) { }

Menuview::~Menuview() { }

void Menuview::draw() { 
	View::draw();
	
	Drawable_list d_list("ul", this->menu->get_id(), this->menu->get_class());
	
	std::vector <Menu_item> items = this->menu->get_items();
	
	std::vector <Menu_item> :: iterator it = items.begin();
	
	for (it = items.begin(); it != items.end(); ++it) {
		if (this->menu->get_selected().get_id() == (it->get_id())) {
			d_list.add_element(it->get_name(), "active");
		} else {
			d_list.add_element(it->get_name(), "normal");
		}
	}
	
	this->style(d_list);
	
	this->draw_element(d_list);
}

void Menuview::load() { 
	View::load("menu");
}
