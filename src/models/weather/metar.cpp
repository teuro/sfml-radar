#include "metar.hpp"

Metar::Metar(std::shared_ptr <Settings> s, std::string i) : settings(s), icao(i) { 	
	this->weather_type = (double)(Tools::linear_random(932, 999)) / 1000.0;
	this->average_pressure = 1040.00 * weather_type;
	this->variation_pressure = (double)(Tools::linear_random(200, 950)) / 100.0 - weather_type;
}

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

void Metar::update_pressure() {
	this->pressure = Tools::normal_distribution(average_pressure, variation_pressure);
}

void Metar::update() {
	this->update_pressure();
	this->time = std::time(0);
	tm *ltm = localtime(&this->time);
	this->id_code = Tools::tostr(ltm->tm_mday, 2) + Tools::tostr(ltm->tm_hour-3, 2) + Tools::tostr(ltm->tm_min, 2) + "z";
	this->clouds.clear();
	this->temperature = Tools::linear_random(-30, 40);
    this->generate_humidity();
    this->generate_visibility();
    this->generate_wind();
    this->devpoint = (int)this->temperature - ((100 - this->humidity) / 5);
	this->generate_clouds();
}

void Metar::generate_visibility() {
	double variation = 100 - ((this->pressure / this->settings->air_pressure_lower) * (this->settings->air_pressure_upper - this->settings->air_pressure_lower));
	double mean_visibility = 6500;
	
	this->visibility = Tools::round_nearest(Tools::normal_distribution(mean_visibility, variation), 100);
}

void Metar::generate_wind() {
	this->wind.direction = Tools::round_nearest(Tools::linear_random(5, 355), 5);
    this->wind.speed = Tools::round_nearest(Tools::linear_random(5, 50), 5);
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
	int max_clouds = Tools::linear_random(0, 3);
	
	while (cloud_iter != cloud_types.begin()) {
		--cloud_iter;
		if (this->pressure > cloud_iter->first && clouds_counter < max_clouds) {
			int altitude = Tools::round_nearest(Tools::linear_random(10, 90), 10);
			std::string name = cloud_iter->second;
				
			Cloud cloud {name, altitude};
			this->clouds.push_back(cloud);
			++clouds_counter;
		}
	}
}

void Metar::generate_humidity() {
	this->humidity = Tools::round_nearest(Tools::normal_distribution(45, 15), 1);
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
