#include "aircraft.hpp"

Aircraft::Aircraft(std::string name, double speed, double heading, double altitude, Coordinate p, int type, Settings& s) : place(p), settings(s) {
    this->name = name;
    this->heading = heading;
    this->altitude = altitude;
    this->speed = speed;

    this->clearance_altitude = altitude;
    this->clearance_heading = heading;
    this->clearance_speed = speed;

    this->separation_error = false;
    this->type = type;
    this->landing = NULL;
    this->target = NULL;
    this->approach = false;
    this->turn = -1;
}

Aircraft::~Aircraft() { }

void Aircraft::update(double elapsed) {
	if (this->type >= 50 && this->speed > 130 && this->altitude < 1000) {
		this->set_clearance_altitude(1500);
	}

    while (this->clearances.size()) {
        Clearance active;
        std::clog << this->clearances.size() << " clearances on stack" << std::endl;
        active = this->clearances.top();
        this->clearances.pop();

        this->handle_clearance(active);
    }

    if (this->target != NULL) {
        //std::clog << this->name << " direct to " << this->target->get_name() << " " << this->target->get_place().get_latitude() << ", " << this->target->get_place().get_longitude() << std::endl;
        this->clearance_heading = Tools::angle(this->place, this->target->get_place());

        if (Tools::on_area(this->place, this->target->get_place())) {
            this->target = NULL;
        }
    }

    double distance = this->speed * (elapsed / 1000) / 3600;

    this->altitude  = change_parameter(elapsed, this->altitude, this->clearance_altitude, 55.0, 0);
    this->speed     = change_parameter(elapsed, this->speed, this->clearance_speed, 3.0, 0);
    this->heading   = change_parameter(elapsed, this->heading, this->clearance_heading, Tools::deg2rad(3.0), this->turn);
	
    this->place = Tools::calculate(this->place, this->heading, distance);

    if (this->approach) {
        std::clog << "Approaching " << this->landing->get_name() << std::endl;
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
    this->landing               = ac.get_landing();
	
    if (this->landing != NULL) {
        std::clog << this->landing->get_name() << std::endl;
        this->approach = true;
    }
}

double Aircraft::change_parameter(double elapsed, double actual_value, double clearance_value, double change, int turn) {
    elapsed /= 1000;
    double add = (clearance_value > actual_value) ? 1.0 : -1.0;
    //std::clog << "Aircraft::change_parameter(" << elapsed << ", " << actual_value << ", " << clearance_value << ", " << change << ", " << turn << ")" << std::endl;
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

void Aircraft::set_target(Navpoint* target) {
    this->target = target;
}

void Aircraft::set_place(Coordinate& place) {
    this->place = place;
}

void Aircraft::set_clearance_speed(double cl_spd) {
	this->clearance_speed = cl_spd;
}

void Aircraft::set_clearance_heading(double cl_hdg) {
	this->clearance_heading = cl_hdg;
}

void Aircraft::set_clearance_altitude(double cl_alt) {
	this->clearance_altitude = cl_alt;
}
