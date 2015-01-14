#include "clearance.hpp"

Clearance::Clearance(int spd, int alt, int hdg, int trn) : speed(spd), altitude(alt), heading(hdg), turn(trn) {
    this->landing = NULL;
}

Clearance::Clearance(Runway*& lnd) : landing(lnd) { }
Clearance::Clearance() { }

Clearance::~Clearance() { }

int Clearance::get_speed() { return this->speed; }

int Clearance::get_altitude() { return this->altitude; }

int Clearance::get_heading() { return this->heading; }

int Clearance::get_turn() { return this->turn; }

Runway*& Clearance::get_landing() {
    return this->landing;
}
