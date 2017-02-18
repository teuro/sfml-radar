#include "metar.hpp"

Metar::Metar() { }

Metar::~Metar() { }

std::string Metar::to_string() {
	std::string clouds;
	
	std::list <Cloud> :: iterator ci;
	
	for (ci = this->clouds.begin(); ci != this->clouds.end(); ++ci) {
		clouds += ci->type + Tools::tostr(ci->altitude, 3) + " ";
	}
	
    return this->icao + " 301250z " + Tools::tostr(wind.direction, 3) + "/" + Tools::tostr(wind.speed, 2) + " KT " + Tools::tostr(visibility) + " " + clouds + " " + Tools::tostr(temperature, 2) + "/" + Tools::tostr(devpoint, 2) + " Q" + Tools::tostr(pressure, 4);
}

void Metar::update(std::string icao) {
	this->clouds.clear();
    this->pressure = Tools::rnd(965, 1035);
    this->humidity = Tools::rnd(20, 80);
    this->visibility = Tools::rnd(100, 9999);
    this->wind.direction = Tools::rnd(5, 355);
    this->wind.speed = Tools::rnd(3, 50);
    this->temperature = Tools::rnd(-30, 40);
    this->devpoint = (int)this->temperature - ((100 - this->humidity) / 5);
	this->icao = icao;
	this->generate_clouds();
}

void Metar::generate_clouds() {
	Cloud cld {"OVC", 30};
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

double Metar::get_wind_direction() {
    return Tools::deg2rad(this->wind.direction);
}

int Metar::get_wind_speed() {
    return this->wind.speed;
}
