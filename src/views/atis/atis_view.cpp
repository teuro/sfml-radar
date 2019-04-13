#include "atis_view.hpp"

Atisview::Atisview(Drawsurface& d, std::shared_ptr <Settings> s, std::shared_ptr <Atis> a) : View(d, s), atis(a) { }

Atisview::~Atisview() { }

void Atisview::load() {
    View::load("atis");
}

void Atisview::draw(Point&) {
	this->set_menu(this->atis->get_menu());
	
	View::draw();
	
	Drawable_list atis_select_runway("ul", this->menu->get_class(), this->menu->get_id());
	
	std::vector <Menu_item> items = this->menu->get_items();
	std::vector <Menu_item> :: iterator it = items.begin();
	
	for (it = items.begin(); it != items.end(); ++it) {
		if (this->menu->get_selected().get_id() == (it->get_id())) {
			atis_select_runway.add_element(it->get_name(), "active");
		} else {
			atis_select_runway.add_element(it->get_name(), "normal");
		}
	}
	
	this->draw_errors();
	
	this->style(atis_select_runway);
	
	this->draw_element(atis_select_runway);
}

std::string Atisview::handle_click(Point&) { return ""; }

void Atisview::draw_errors() {
	Drawable_list atis_errors("ul", "list", "atis_error");
	std::list <std::string> errors = this->atis->get_atis_errors();
	std::list <std::string> :: iterator it = errors.begin();
	
	while (it != errors.end()) {
		atis_errors.add_element((*it));
		++it;
	}
	
	this->style(atis_errors);
	this->draw_element(atis_errors);
}

void Atisview::draw_runways() {
	#ifdef DEBUG
	std::clog << "Atisview::draw_runways()" << std::endl;
	#endif
	
	Drawable_list list_runways("ul", "atis_base", "runways");
	
	for (auto v: this->atis->get_runways()) {
		list_runways.add_element(v.get_name(), "departure", "");
	}
	
	this->draw_element(list_runways);
}

void Atisview::update() {

}