#include "aircraft.hpp"

Aircraft::Aircraft(std::string name, double speed, double heading, double altitude, Coordinate p, int type, Settings& s, Runway& land) : place(p), settings(s), landing(land) {
    this->name = name;
    this->heading = heading;
    this->altitude = altitude;
    this->speed = speed;

    this->clearance_altitude = altitude;
    this->clearance_heading = heading;
    this->clearance_speed = speed;

    this->separation_error = false;
    this->type = type;
    this->target = NULL;
    this->approach = false;
    this->turn = -1;
}

Aircraft::~Aircraft() { }

void Aircraft::update(double elapsed) {
	while (this->heading > 2 * Tools::get_PI()) {
		this->heading -= 2 * Tools::get_PI();
	}
	
	while (this->heading < 0) {
		this->heading += 2 * Tools::get_PI();
	}

	if (this->approach) {
		if (this->landed == false) {
			double t_angle = Tools::angle(this->place, this->landing.get_start_place());
			double t_distance = Tools::distanceNM(this->place, this->landing.get_start_place()) * 6076.11549;
			double target_approach_altitude = std::tan(Tools::deg2rad(this->landing.get_glidepath())) * t_distance;
			double min_approach_angle = this->landing.get_heading() - Tools::deg2rad(this->settings.approach_angle);
			double max_approach_angle = this->landing.get_heading() + Tools::deg2rad(this->settings.approach_angle);
		
			if (this->altitude > this->settings.max_approach_altitude || this->heading > max_approach_angle || this->heading <  min_approach_angle || this->speed > this->settings.max_approach_speed) {
				std::cerr << "Plane must be " << this->settings.max_approach_altitude << ", speed must max " << this->settings.max_approach_speed << " kt and approach angle must be between " << Tools::rad2deg(min_approach_angle) << " and " << Tools::rad2deg(max_approach_angle) << std::endl;
				this->approach = false;
				return;
			}
		
			if (this->altitude < this->settings.max_approach_altitude && this->altitude > target_approach_altitude) {
				if (this->altitude < this->settings.airfield_altitude) {
					this->altitude = this->settings.airfield_altitude;
					this->landed = true;
				} else {
					this->altitude = target_approach_altitude;
				}
			}
		
			this->clearance_heading = t_angle;
		} else {
			this->clearance_speed = 0;
		}
	} else {
		if (this->type >= 50 && this->speed > 130 && this->altitude < 1000) {
			this->set_clearance_altitude(1500);
		}

		if (this->target != NULL) {
			this->clearance_heading = Tools::angle(this->place, this->target->get_place());

			if (Tools::on_area(this->place, this->target->get_place())) {
				this->target = NULL;
			}
		}
	}
	
	this->altitude  = change_parameter(elapsed, this->altitude, this->clearance_altitude, 	30.0, 0);
	this->speed     = change_parameter(elapsed, this->speed, 	this->clearance_speed, 		3.0, 0);
	this->heading   = change_parameter(elapsed, this->heading, 	this->clearance_heading, 	Tools::deg2rad(3.0), this->turn);
	
	double distance = this->speed * (elapsed / 1000) / 3600;
    this->place = Tools::calculate(this->place, this->heading, distance);
}

Coordinate& Aircraft::get_place() {
    return this->place;
}

std::string Aircraft::get_name() {
    return this->name;
}

double Aircraft::change_parameter(double elapsed, double actual_value, double clearance_value, double change, int turn) {
    elapsed /= 1000.0;
	double add = turn;
	
	if (add == 0) {
		add = (clearance_value > actual_value) ? 1.0 : -1.0;
	}
	
    if (std::abs(actual_value - clearance_value) < (0.01 * clearance_value)) {
        return clearance_value;
    } else {
        actual_value += elapsed * change * add;
    }

    return actual_value;
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

void Aircraft::set_clearance_heading(double cl_hdg, int turn) {
	this->clearance_heading = cl_hdg;
	this->turn = turn;
}

void Aircraft::set_clearance_altitude(double cl_alt) {
	this->clearance_altitude = cl_alt;
}

void Aircraft::set_clearance_approach() {
	this->approach = true;
}
