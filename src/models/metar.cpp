#include "metar.hpp"

Metar::Metar() { }

Metar::~Metar() { }

std::string Metar::to_string() {
	std::string clouds;
	
	std::list <Cloud> :: iterator ci;
	
	for (ci = this->clouds.begin(); ci != this->clouds.end(); ++ci) {
		clouds += ci->type + Tools::tostr(ci->altitude) + " ";
	}
	
    return this->icao + " 301250z " + Tools::tostr(wind.direction) + Tools::tostr(wind.speed) + " KT " + Tools::tostr(visibility) + " " + clouds + " " + Tools::tostr(temperature) + "/" + Tools::tostr(devpoint) + " Q" + Tools::tostr(pressure);
}

void Metar::update(std::string icao) {
	this->clouds.clear();
    this->pressure = 1013;
    this->humidity = 65;
    this->visibility = 8500;
    this->wind.direction = 250;
    this->wind.speed = 12;
    this->devpoint = 03;
    this->temperature = 02;
	this->icao = icao;
	this->generate_clouds();
}

void Metar::generate_clouds() {
	Cloud cld {"BKN", 30};
    this->clouds.push_back(cld);
}

int Metar::get_pressure() {
    return this->pressure;
}

int Metar::get_temperature() {
    return this->temperature;
}

int Metar::get_visibility() {
    return this->visibility;
}

int Metar::get_wind_direction() {
    return this->wind.direction;
}

int Metar::get_correct_level(int transition_altitude) {
    Queryresult result = Database::get_result("SELECT level FROM pressure_limit, transfer_levels WHERE " + Tools::tostr(pressure) + " BETWEEN lower AND upper AND altitude = " + Tools::tostr(transition_altitude) + " AND pressure_limit.ROWID = transfer_levels.pressure_id");
    return Tools::toint(result(0, "level"));
}
