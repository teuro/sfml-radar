#include "clearance.hpp"

Clearance::Clearance(int spd, int alt, int hdg, int trn) : speed(spd), altitude(alt), heading(hdg), turn(trn) { }

Clearance::~Clearance() { }

Clearance::Clearance() { }

int Clearance::get_speed() { return this->speed; }

int Clearance::get_altitude() { return this->altitude; }

int Clearance::get_heading() { return this->heading; }

int Clearance::get_turn() { return this->turn; }
