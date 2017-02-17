#include "atis.hpp"

Atis::Atis(Settings& s, Metar& m) : settings(s), metar(m) { 
	this->departure_runway = "";
	this->landing_runway = "";
	this->transition_altitude = 0;
	this->transition_level = 0;
}

Atis::~Atis() { }

void Atis::load(std::vector <Runway> rwys) {
	this->runways = rwys;
}

void Atis::update() {
	
}

void Atis::set_departure_runway(std::string dep_rwy) {
    this->departure_runway = dep_rwy;
}

void Atis::set_landing_runway(std::string lnd_rwy) {
    this->landing_runway = lnd_rwy;
}

void Atis::set_transition_altitude(int tr_alt) {
    this->transition_altitude = tr_alt;
}

void Atis::set_transition_level(int tr_lvl) {
    this->transition_level = tr_lvl;
}

std::string Atis::get_departure_runway() {
    return this->departure_runway;
}

std::string Atis::get_landing_runway() {
    return this->landing_runway;
}

int Atis::get_transition_level() {
    return this->transition_level;
}

int Atis::get_transition_altitude() {
    return this->transition_altitude;
}

int Atis::calculate_tr_level(int pressure, int altitude) {
	//std::clog << "Atis::calculate_tr_level(" << pressure << ", " << altitude << ")" << std::endl;
	std::map <int, std::vector <int> > levels;
	int place = -1;
	
	levels[3000].push_back(25);
	levels[3000].push_back(30);
	levels[3000].push_back(35);
	levels[3000].push_back(40);
	levels[3000].push_back(45);
	levels[3000].push_back(50);
	
	levels[4000].push_back(35);
	levels[4000].push_back(40);
	levels[4000].push_back(45);
	levels[4000].push_back(50);
	levels[4000].push_back(55);
	levels[4000].push_back(60);
	
	levels[5000].push_back(45);
	levels[5000].push_back(50);
	levels[5000].push_back(55);
	levels[5000].push_back(60);
	levels[5000].push_back(65);
	levels[5000].push_back(70);
	
	levels[6000].push_back(55);
	levels[6000].push_back(60);
	levels[6000].push_back(65);
	levels[6000].push_back(70);
	levels[6000].push_back(75);
	levels[6000].push_back(80);
	
	levels[18000].push_back(175);
	levels[18000].push_back(180);
	levels[18000].push_back(185);
	levels[18000].push_back(190);
	levels[18000].push_back(195);
	levels[18000].push_back(200);
	
	if (pressure >= 943 && pressure < 959) {
		place = 5;
	} else if (pressure >= 960 && pressure < 977) {
		place = 4;
	} else if (pressure >= 978 && pressure < 995) {
		place = 3;
	} else if (pressure >= 996 && pressure < 1013) {
		place = 2;
	} else if (pressure >= 1014 && pressure < 1031) {
		place = 1;
	} else if (pressure >= 1032 && pressure < 1050) {
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

double Atis::calculate_backwind(std::string runway_name) {
	//std::clog << "Atis::calculate_backwind(" << runway_name << ")" << std::endl;
	std::vector <Runway> :: iterator it_rwy = std::find(this->runways.begin(), this->runways.end(), runway_name);
	
	return this->calculate_backwind(it_rwy->get_heading());
}

bool Atis::check_backwind(std::string runway_name) {
	//std::clog << "Atis::check_backwind(" << runway_name << ")" << std::endl;
	if (runway_name.length() > 0) {
		std::vector <Runway> :: iterator it_rwy = std::find(this->runways.begin(), this->runways.end(), runway_name);
		
		if (this->calculate_backwind(it_rwy->get_heading()) > 0) {
			return false;
		}
		
		return true;
	}
	
	return true;
}

bool Atis::ok() {
	this->atis_errors.clear();
	bool ok = true;
	
	if (transition_altitude > 2000 && transition_altitude < 19000) {
		int calculated_tr_level = this->calculate_tr_level(this->metar.get_pressure(), transition_altitude);
		
		if (calculated_tr_level != transition_level) {
			this->atis_errors.push_back("transition level should be " + Tools::tostr(calculated_tr_level));
			ok = false;
		}
	} else {
		this->atis_errors.push_back("Choose transition altitude");
		ok = false;
	}
	
	if (departure_runway.length()) {
		if (!check_backwind(departure_runway)) {
			this->atis_errors.push_back("runway " + departure_runway + " has " + Tools::tostr(calculate_backwind(departure_runway)) + " kt wind speed");
			ok = false;
		} 
	} else {
		this->atis_errors.push_back("Choose runway for departure");
		ok = false;
	}
	
	if (landing_runway.length()) {
		if (!check_backwind(landing_runway)) {
			this->atis_errors.push_back("runway " + landing_runway + " has " + Tools::tostr(calculate_backwind(landing_runway)) + " kt wind speed");
			ok = false;
		}
	} else {
		this->atis_errors.push_back("Choose runway for landing");
		ok = false;
	}
	
	return ok;
}
