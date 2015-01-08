#include "aircraft.hpp"

Aircraft::Aircraft(std::string name, int speed, int heading, int altitude, Coordinate p, int type) : place(p) {
    this->name = name;
    this->heading = heading;
    this->altitude = altitude;
    this->speed = speed;

    this->clearance_altitude = altitude;
    this->clearance_heading = heading;
    this->clearance_speed = speed;

    this->separation_error = false;
    this->type = type;
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
    this->place = Tools::calculate(this->place, Tools::deg2rad(this->heading), distance);

    this->altitude  = change_parameter(elapsed, this->altitude, this->clearance_altitude, 11.0, 0);
    this->speed     = change_parameter(elapsed, this->speed, this->clearance_speed, 3.0, 0);
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

double Aircraft::change_parameter(double elapsed, double actual_value, double clearance_value, double change, int turn) {
    elapsed /= 1000;
    double add = (clearance_value > actual_value) ? 1.0 : -1.0;
    //std::clog << "Aircraft::chang_parameter(" << elapsed << ", " << actual_value << ", " << clearance_value << ", " << change << ", " << turn << ")" << std::endl;
    if (std::abs(actual_value - clearance_value) < 1.3) {
        //std::clog << "Actual value is closer to clearance value than 1.3 unit " << std::endl;
        return clearance_value;
    } else if (turn != 0) {
        //std::clog << "Turn is " << turn << std::endl;
        actual_value += elapsed * change * (double)turn;
        //std::clog << "Turn is not 0 " << actual_value << std::endl;
    } else {
        actual_value += elapsed * change * add;
        //std::clog << "Speed or altitude" << actual_value << std::endl;
    }

    return actual_value;
}

void Aircraft::set_clearance(Clearance& cl) {
    this->clearances.push(cl);
}

double Aircraft::get_speed() {
    return this->speed;
}

double Aircraft::get_heading() {
    return this->heading;
}

double Aircraft::get_altitude() {
    return this->altitude;
}

int Aircraft::get_turn() {
    return this->turn;
}

double Aircraft::get_clearance_speed() {
    return this->clearance_speed;
}

double Aircraft::get_clearance_heading() {
    return this->clearance_heading;
}

double Aircraft::get_clearance_altitude() {
    return this->clearance_altitude;
}

void Aircraft::set_separation_error(bool t) {
    this->separation_error = t;
}

bool Aircraft::get_separation_error() {
    return this->separation_error;
}

int Aircraft::get_type() {
    return this->type;
}
