#include "menu_view.hpp"

Menuview::Menuview(Drawsurface& d, std::shared_ptr <Settings> s, std::shared_ptr <Menu> m) : View(d, s), menu(m) { 
	#ifdef DEBUG
	std::clog << "Menuview::" << menu->get_items().size() <<  std::endl;
	#endif
}

Menuview::~Menuview() { }

std::string Menuview::handle_click(Point& mouse) { 
	for (unsigned int i = 0; i < this->clicks.size(); ++i) {
		Point place = this->clicks[i].get_style().get_place();
		
		if (Tools::on_area(mouse, place, 20)) {
			this->clicks[i].change_value("required_handled", Tools::toint(this->clicks[i].get_name()));
		}
	}
	
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
	int x = 15;
	int y = 30;
	
	for (unsigned int i = 0; i < this->clicks.size(); ++i) {
		this->style(this->clicks[i]);
		
		this->clicks[i].get_style().set_attribute("left", x);
		this->clicks[i].get_style().set_attribute("top", y);
		
		this->drawer.draw_text(
			this->clicks[i].get_name(), 
			this->clicks[i].get_style().get_place(),
			this->clicks[i].get_style().get_text_color()
		);
		
		y += 20;
	}
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
