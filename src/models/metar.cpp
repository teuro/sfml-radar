#include "metar.hpp"

Metar::Metar() { }

Metar::~Metar() { }

std::string Metar::get_metar() {
    return "";
}

void Metar::update() {
    this->pressure = 997;
    this->humidity = 65;
    this->visibility = 8500;
    this->wind.direction = 250;
    this->wind.speed = 12;
    Cloud cld {"BKN", 030};
    this->clouds.push_back(cld);
}
