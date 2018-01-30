#include "menu_view.hpp"

Menuview::Menuview(Drawsurface& d, Settings& s, Menu*& m) : View(d, s), menu(m) { 
	#ifdef DEBUG
	std::clog << "Menuview::" << menu->get_items().size() <<  std::endl;
	#endif
}

Menuview::~Menuview() { }

void Menuview::draw() { 
	#ifdef DEBUG
	std::clog << "Menuview::draw()" << std::endl;
	#endif
	
	View::draw();
	
	#ifdef DEBUG
	std::clog << "View::drawed" << std::endl;
	#endif
	
	Drawable_list d_list("ul", this->menu->get_class(), this->menu->get_id());
	
	#ifdef DEBUG
	std::clog << "Menuview::drawable list created" << std::endl;
	#endif
	
	std::vector <Menu_item> items = this->menu->get_items();
	
	#ifdef DEBUG
	std::clog << "Menuview::" << items.size() <<  std::endl;
	#endif
	
	std::vector <Menu_item> :: iterator it = items.begin();
	
	#ifdef DEBUG
	std::clog << "Menuview::" << items.size() <<  std::endl;
	#endif
	
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
	std::clog << "Menuview::load()" << std::endl;
	View::load("menu");
}

void Menuview::set_menu(Menu*& menu) {
	this->menu = menu;
}
