#include "atis_view.hpp"

Atisview::Atisview(Drawsurface& d, Settings& s, Atis*& a) : View(d, s), atis(a) { }

Atisview::~Atisview() { }

void Atisview::load(std::vector <Runway> runways, std::map <int, std::vector <int> > levels) {
    View::load("atis");
	this->tr_levels = levels;
	this->runways = runways;
}

void Atisview::update() {
	Drawable_list runway_list_dep("ul", "atis_base", "runway-list-departure");
	Drawable_list runway_list_lnd("ul", "atis_base", "runway-list-landing");
	Drawable_list transfer_level("ul", "atis_base",  "transfer-level");
	Drawable_list transfer_altitude("ul", "atis_base", "transfer-altitude");
	
	runway_list_dep.add_element("departure");
	runway_list_lnd.add_element("landing");
	transfer_altitude.add_element("altitude");
	
	for (unsigned int i = 0; i < runways.size(); ++i) {
		runway_list_dep.add_element(runways[i].get_name());
		runway_list_lnd.add_element(runways[i].get_name());
	}

	std::map <int, std::vector <int> > :: iterator altitude;
	std::vector <int> :: iterator level;
	
	for (altitude = tr_levels.begin(); altitude != tr_levels.end(); ++altitude) {
		transfer_altitude.add_element(Tools::tostr(altitude->first));
		level = altitude->second.begin();
		transfer_level.add_element("level");
		
		while (level != altitude->second.end()) {
			transfer_level.add_element(Tools::tostr((*level)));
			++level;
		}
		
		display_levels[altitude->first] = transfer_level;
		transfer_level.clear_content();
	}

	int t_altitude = this->atis->get_transition_altitude();
	
	this->style(runway_list_dep);
	this->style(runway_list_lnd);
	this->style(transfer_altitude);
	
	/*this->draw_element(runway_list_dep);
	this->draw_element(runway_list_lnd);
	this->draw_element(transfer_altitude);*/
	
	if (t_altitude > 2000) {
		this->style(display_levels[t_altitude]);
		//this->draw_element(display_levels[t_altitude]);
	}
}

void Atisview::draw() {
	this->update();
	View::draw();
	
	for (unsigned int i = 0; i < this->lists.size(); ++i) {
		this->style(this->lists[i]);
		this->draw_element(this->lists[i]);
	}
}

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