#include "aircraft.hpp"

Aircraft::Aircraft(std::string name, int speed, int heading, int altitude, Coordinate p) : place(p) {
    this->name = name;
    this->heading = heading;
    this->altitude = altitude;
    this->speed = speed;

    this->clearance_altitude = altitude;
    this->clearance_heading = heading;
    this->clearance_speed = speed;

    this->separation_error = false;
}

Aircraft::~Aircraft() { }

void Aircraft::update(double elapsed) {
    while (this->clearances.size()) {
        Clearance active;
        std::clog << this->clearances.size() << " clearances on stack" << std::endl;
        active = this->clearances.top();
        this->clearances.pop();

        this->handle_clearance(active);
    }

    double distance = this->speed * (elapsed / 1000) / 3600;
    this->place = Tools::calculate(this->place, this->heading, distance, false, true);

    this->altitude  = change_parameter(elapsed, this->altitude, this->clearance_altitude, 11.0);
    this->speed     = change_parameter(elapsed, this->speed, this->clearance_speed, 3.0);
    this->heading   = change_parameter(elapsed, this->heading, this->clearance_heading, 3.0, this->turn);

    while (this->heading < 0.0) {
        this->heading += 360.0;
    }

    while (this->heading > 360.0) {
        this->heading -= 360.0;
    }
}

Coordinate& Aircraft::get_place() {
    return this->place;
}

std::string Aircraft::get_name() {
    return this->name;
}

void Aircraft::handle_clearance(Clearance& ac) {
    this->clearance_speed       = ac.get_speed();
    this->clearance_altitude    = ac.get_altitude();
    this->clearance_heading     = ac.get_heading();
    this->turn                  = ac.get_turn();

    while (this->clearance_heading < 0.0) {
        this->clearance_heading += 360.0;
    }

    while (this->clearance_heading > 360.0) {
        this->clearance_heading -= 360.0;
    }
}

double Aircraft::change_parameter(double elapsed, double value, double _value, double change, int turn) {
    elapsed /= 1000;

    if (std::abs(value - _value) < 5) {
        return _value;
    } else {
        double direction;

        if (turn == 0) {
            direction = (value > _value) ? -1.0 : 1.0;
        } else {
            direction = (double)turn;
        }
        value += elapsed * change * direction;
    }

    return value;
}

void Aircraft::set_clearance(Clearance& cl) {
    this->clearances.push(cl);
}
