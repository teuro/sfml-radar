#include "atis.hpp"

Atis::Atis(Settings& s, Metar& m) : settings(s), metar(m) { 
	this->departure_runway = "22R";
	this->landing_runway = "22L";
	this->transition_altitude = 5000;
	//this->transition_level = 55;
}

Atis::~Atis() { }

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
	
	if (pressure > 1032 && pressure < 1050) {
		place = 0;
	} else if (pressure > 1014 && pressure < 1031) {
		place = 1;
	} else if (pressure > 996 && pressure < 1013) {
		place = 2;
	} else if (pressure > 978 && pressure < 995) {
		place = 3;
	} else if (pressure > 960 && pressure < 977) {
		place = 4;
	} else if (pressure > 943 && pressure < 959) {
		place = 5;
	}
	
	return levels[altitude][place];
}

bool Atis::ok() {
	//std::clog << this->calculate_tr_level(this->metar.get_pressure(), transition_altitude) << std::endl;
	if (transition_level > 10 && transition_level < 70) {
		if (transition_altitude > 2000 && transition_altitude < 8000) {
			if (departure_runway.length() > 0) {
				if (landing_runway.length() > 0) {
					return true;
				}
			}
		}
	}
	
	return false;
}
