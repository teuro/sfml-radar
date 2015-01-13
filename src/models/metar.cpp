#include "metar.hpp"

Metar::Metar() { }

Metar::~Metar() { }

std::string Metar::get_metar() {
    return ICAO + " 301250z " + Tools::tostr(wind.direction) + Tools::tostr(wind.speed) + "KT " + Tools::tostr(visibility) + " " + Tools::tostr(temperature) + "/" + Tools::tostr(devpoint) + "Q" + Tools::tostr(pressure);
}

void Metar::update() {
    this->pressure = 1013;
    this->humidity = 65;
    this->visibility = 8500;
    this->wind.direction = 250;
    this->wind.speed = 12;
    Cloud cld {"BKN", 030};
    this->clouds.push_back(cld);
    this->devpoint = 03;
    this->temperature = 02;
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
    return Tools::tonumber<int>(result(0, "level"));
}
