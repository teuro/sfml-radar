#include "atis_view.hpp"

Atisview::Atisview(Drawsurface& d, std::shared_ptr <Settings> s, std::shared_ptr <Atis> a) : View(d, s), atis(a) { }

Atisview::~Atisview() { }

void Atisview::load() {
    View::load("atis");
}

void Atisview::draw(Point&) {
	this->set_menu(this->atis->get_menu());
	
	View::draw();
	
	this->draw_menu(this->menu);
	
	this->draw_errors();
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