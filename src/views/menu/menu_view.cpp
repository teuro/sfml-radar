#include "menu_view.hpp"

Menuview::Menuview(Drawsurface& d, std::shared_ptr <Settings> s, std::shared_ptr <Menu> m) : View(d, s), menu(m) { 
	#ifdef DEBUG
	std::clog << "Menuview::" << menu->get_items().size() <<  std::endl;
	#endif
}

Menuview::~Menuview() { }

std::string Menuview::handle_click(Point& ) { 
	
	return "";
}

void Menuview::draw(Point&) { 
	#ifdef DEBUG
	std::clog << "Menuview::draw()" << std::endl;
	#endif
	
	View::draw();
	
	#ifdef DEBUG
	std::clog << "View::drawed" << std::endl;
	#endif
	
	this->draw_menu(this->menu);
}

void Menuview::load() { 
	#ifdef DEBUG
	std::clog << "Menuview::load()" << std::endl;
	#endif
	
	View::load("menu");
}

void Menuview::set_menu(std::shared_ptr <Menu> menu) {
	this->menu = menu;
}

void Menuview::update() { }
