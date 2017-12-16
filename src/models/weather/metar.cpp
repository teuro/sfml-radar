#include "metar.hpp"

Metar::Metar() { }

Metar::~Metar() { }

std::string Metar::to_string() {
	std::string clouds;
	
	std::list <Cloud> :: iterator ci;
	
	for (ci = this->clouds.begin(); ci != this->clouds.end(); ++ci) {
		if (ci->type != "SKC" && ci->type != "CLR") {
			clouds += ci->type + Tools::tostr(ci->altitude, 3) + " ";
		} else {
			clouds += ci->type + " ";
		}
	}
	
    return this->icao + " " + this->id_code + " " + Tools::tostr(wind.direction, 3) + "/" + Tools::tostr(wind.speed, 2) + " KT " + Tools::tostr(visibility) + " " + clouds + " " + Tools::tostr(temperature, 2) + "/" + Tools::tostr(devpoint, 2) + " Q" + Tools::tostr(pressure, 4);
}

void Metar::update(std::string icao) {
	this->time = std::time(0);
	tm *ltm = localtime(&this->time);
	this->id_code = Tools::tostr(ltm->tm_mday) + Tools::tostr(ltm->tm_hour-3) + Tools::tostr(ltm->tm_min) + "z";
	this->clouds.clear();
    this->pressure = Tools::rnd(965, 1035);
	this->temperature = Tools::rnd(-30, 40);
    this->generate_humidity();
    this->generate_visibility();
    this->generate_wind();
    this->devpoint = (int)this->temperature - ((100 - this->humidity) / 5);
	this->icao = icao;
	this->generate_clouds();
}

double f(double x) {
	return std::pow(x, 0.3285155592603834);
}

double result(double x, double min_return, double max_return) {
	double min = 960;
	double  max = 1050;
	if (x == min) return min_return;
	if (x == max) return max_return;
	return result(min, min_return, max_return) + (f(x) - f(min)) / (f(max) - f(min)) * (result(max, min_return, max_return) - result(min, min_return, max_return));
}

void Metar::generate_visibility() {
	this->visibility = Tools::round_nearest(result(this->pressure, 100, 9999), 100);
}

void Metar::generate_wind() {
	this->wind.direction = Tools::round_nearest(Tools::rnd(5, 355), 5);
    this->wind.speed = Tools::round_nearest(result(this->pressure, 5, 50), 5);
}

void Metar::generate_clouds() {
	std::map <int, std::string> cloud_types;
	cloud_types[1040] = "SKC";
	cloud_types[1035] = "CLR";
	cloud_types[1030] = "NSC";
	cloud_types[1025] = "FEW";
	cloud_types[1020] = "SCT";
	cloud_types[980] = "BKN";
	cloud_types[970] = "OVC";
	cloud_types[960] = "VV";
	
	std::map <int, std::string> :: iterator cloud_iter = cloud_types.end();
	int clouds_counter = 0;
	int max_clouds = 1;
	
	while (cloud_iter != cloud_types.begin()) {
		--cloud_iter;
		if (this->pressure > cloud_iter->first && clouds_counter < max_clouds) {
			int altitude = Tools::round_nearest(result(this->pressure, 10, 90), 10);
			std::string name = cloud_iter->second;
				
			Cloud cld {name, altitude};
			this->clouds.push_back(cld);
			++clouds_counter;
		}
	}
}

void Metar::generate_humidity() {
	this->humidity = Tools::round_nearest(result(this->pressure, 5, 80), 1);
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
