#include "atis.hpp"

Atis::Atis(Settings& s, Metar& m) : settings(s), metar(m) { 
	this->transition_altitude = 0;
	this->transition_level = 0;
	
	this->state = DEPARTURE;
	this->menu = new Menu("runway", "atis_base");
}

Atis::~Atis() { }

void Atis::load() {	
	std::clog << "Atis::load()" << std::endl;
	altitudes.push_back(3000);
	altitudes.push_back(4000);
	altitudes.push_back(5000);
	altitudes.push_back(6000);
	altitudes.push_back(7000);
	altitudes.push_back(18000);
	
	for (unsigned int i = 0; i < altitudes.size(); ++i) {
		int base = altitudes[i] / 100 - 5;
		
		for (int j = 1;  j < this->settings.transition_levels; ++j) {
			this->levels[altitudes[i]].push_back(base);
			base += 5;
		}
	}
}

std::vector <Runway> Atis::get_runways() {
	return this->active_field->get_runways();
}

std::vector <int> Atis::get_altitudes() {
	return this->altitudes;
}

std::vector <int> Atis::get_levels(int altitude) {
	return this->levels[altitude];
}

void Atis::update(int amount) {
	this->menu->change_selection(amount);
}

Menu* Atis::get_menu() {
	return this->menu;
}

void Atis::set_departure_runway(std::string dep_rwy) {
	std::vector <Runway> :: iterator it_rwy = std::find(this->active_field->get_runways().begin(), this->active_field->get_runways().end(), dep_rwy);
	
	this->departure_runway = (*it_rwy);
}

void Atis::set_landing_runway(std::string lnd_rwy) {
	std::vector <Runway> :: iterator it_rwy = std::find(this->active_field->get_runways().begin(), this->active_field->get_runways().end(), lnd_rwy);
	
	this->landing_runway = (*it_rwy);
}

void Atis::set_transition_altitude(int tr_alt) {
	//std::clog << "Atis::set_transition_altitude(" <<  tr_alt << ")" << std::endl; 
    this->transition_altitude = tr_alt;
}

void Atis::set_transition_level(int tr_lvl) {
	//std::clog << "Atis::set_transition_level(" <<  tr_lvl << ")" << std::endl; 
    this->transition_level = tr_lvl;
}

Runway& Atis::get_departure_runway() {
    return this->departure_runway;
}

Runway& Atis::get_landing_runway() {
    return this->landing_runway;
}

int Atis::get_transition_level() {
    return this->transition_level;
}

int Atis::get_transition_altitude() {
    return this->transition_altitude;
}

std::map <int, std::vector <int> > Atis::get_levels() {
	return this->levels;
}

int Atis::calculate_tr_level(int pressure, int altitude) {
	int place = -1;

	if (pressure >= 943 && pressure < 959) {
		place = 5;
	} else if (pressure >= 959 && pressure < 977) {
		place = 4;
	} else if (pressure >= 977 && pressure < 995) {
		place = 3;
	} else if (pressure >= 995 && pressure < 1013) {
		place = 2;
	} else if (pressure >= 1013 && pressure < 1031) {
		place = 1;
	} else if (pressure >= 1031 && pressure < 1050) {
		place = 0;
	} 
		
	return levels[altitude][place];
}

std::list <std::string>  Atis::get_atis_errors() {
	return this->atis_errors;
}

double Atis::calculate_backwind(double runway) {
	//std::clog << "Atis::calculate_backwind(" << runway << ")" << std::endl;
	double wind = this->metar.get_wind_direction();
	int speed = this->metar.get_wind_speed();
	
	return -std::cos(wind - runway) * speed;
}

double Atis::calculate_backwind(Runway& runway) {
	//std::clog << "Atis::calculate_backwind(" << runway_name << ")" << std::endl;
	std::vector <Runway> :: iterator it_rwy = std::find(this->active_field->get_runways().begin(), this->active_field->get_runways().end(), runway.get_name());
	
	return this->calculate_backwind(it_rwy->get_heading());
}

bool Atis::check_backwind(Runway& runway) {
	//std::clog << "Atis::check_backwind(" << runway_name << ")" << std::endl;
	if (runway.get_name().length() > 0) {
		
		if (this->calculate_backwind(runway.get_heading()) > 0) {
			return false;
		}
		
		return true;
	}
	
	return true;
}

bool Atis::departure_runway_ok() {
	bool ok = true;
	
	if (departure_runway.get_name().length()) {
		if (!check_backwind(departure_runway)) {
			this->atis_errors.push_back("runway " + departure_runway.get_name() + " has " + Tools::tostr(calculate_backwind(departure_runway)) + " kt tailwind");
			ok = false;
		} 
	} else {
		this->atis_errors.push_back("Choose runway for departure");
		ok = false;
	}
	
	if (ok) {
		this->state = LANDING;
	}
	
	return ok;
}

bool Atis::landing_runway_ok() {
	bool ok = true;
	
	if (landing_runway.get_name().length()) {
		if (!check_backwind(landing_runway)) {
			this->atis_errors.push_back("runway " + landing_runway.get_name() + " has " + Tools::tostr(calculate_backwind(landing_runway)) + " kt tailwind");
			ok = false;
		}
	} else {
		this->atis_errors.push_back("Choose runway for landing");
		ok = false;
	}
	
	if (ok) {
		this->state = ALTITUDE;
		this->menu->clear();
		
		for (auto altitude: this->altitudes) {
			this->menu->add_item(Tools::tostr(altitude));
		}
	}
	
	return ok;
}

bool Atis::transition_altitude_ok() {
	bool ok = true;
	
	if (transition_altitude < 2000 || transition_altitude > 19000) {
		this->atis_errors.push_back("Choose transition altitude");
		ok = false;
	}
	
	if (ok) {
		this->state = LEVEL;
		
		this->menu->clear();
		std::vector <int> tmp_levels = this->levels[this->transition_altitude];
		
		for (auto level: tmp_levels) {
			this->menu->add_item(Tools::tostr(level));
		}
	}
	
	return ok;
}

bool Atis::transfer_level_ok() {
	bool ok = true;
	
	int calculated_tr_level = this->calculate_tr_level(this->metar.get_pressure(), transition_altitude);
		
	if (calculated_tr_level != transition_level) {
		this->atis_errors.push_back("transition level should be " + Tools::tostr(calculated_tr_level));
		ok = false;
	}
	
	return ok;
}

bool Atis::ok() {
	this->atis_errors.clear();
	bool ok = true;
	
	ok = this->departure_runway_ok();
	ok = this->landing_runway_ok();
	ok = this->transition_altitude_ok();
	
	if (ok) {
		ok = this->transfer_level_ok();
	}
	
	return ok;
}

void Atis::set_value(std::string value) {
	switch (this->state) {
	case DEPARTURE:
		this->set_departure_runway(value);
		break;
	case LANDING:
		this->set_landing_runway(value);
		break;
	case ALTITUDE:
		this->set_transition_altitude(Tools::toint(value));
		break;
	case LEVEL:
		this->set_transition_level(Tools::toint(value));
		break;
	}
}

void Atis::set_airfield(std::shared_ptr <Airfield> airfield) {
	this->active_field = airfield;
	
	for (auto name: this->active_field->get_runways()) {
		this->menu->add_item(name.get_name());
	}
}
