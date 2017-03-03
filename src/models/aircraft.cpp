#include "aircraft.hpp"

Aircraft::Aircraft(std::string t_name, Settings& s, Airfield*& af, Atis*& a, Inpoint& ip) : name(t_name), settings(s), airport(af), atis(a) {
	this->place = ip.get_place();
	this->heading = ip.get_heading();
	this->altitude = ip.get_altitude();
	this->speed = 250;
	this->type = APPROACH;
	this->load();
}

Aircraft::Aircraft(std::string t_name, Settings& s, Airfield*& af, Atis*& a, Outpoint& op) : name(t_name), settings(s), airport(af), atis(a) {
	this->place = this->airport->get_runway(this->atis->get_departure_runway()).get_start_place();
	this->altitude = this->settings.airfield_altitude;
	this->heading = this->airport->get_runway(this->atis->get_departure_runway()).get_heading();
	this->speed = 0;
	this->target = op;
	this->type = DEPARTURE;
	this->load();
}

void Aircraft::load() {
	this->clearance_altitude = -1;
    this->clearance_heading = -1;
    this->clearance_speed = -1;

    this->separation_error = false;
    this->approach = false;
    this->landed = false;
    this->direct = false;
	this->expect = false;
	this->final_approach = false;
	
    this->turn = -1;
}

Aircraft::~Aircraft() { }

void Aircraft::set_approach_runway(std::string name) {
	try {
		this->landing = this->airport->get_runway(name);
		this->expect = true;
	} catch (std::logic_error& e) {
		std::cerr << e.what() << std::endl;
	}
}

bool Aircraft::check_approach_config() {
	double min_approach_angle = this->landing.get_heading() - Tools::deg2rad(this->settings.approach_angle);
	double max_approach_angle = this->landing.get_heading() + Tools::deg2rad(this->settings.approach_angle);
	
	if (this->altitude > this->settings.max_approach_altitude) {
		std::clog << "plane altitude is " << this->altitude << " ft " << this->settings.max_approach_altitude << " ft" << std::endl;
		
		return false;
		
		if (this->heading > max_approach_angle || this->heading <  min_approach_angle) {
			std::clog << "max approach angle " << Tools::rad2deg(max_approach_angle) << " < " << Tools::rad2deg(this->heading) << std::endl;
			std::clog << "min approach angle " << Tools::rad2deg(min_approach_angle) << " < " << Tools::rad2deg(this->heading) << std::endl;
			return false;
		}
		
		if (this->speed > this->settings.max_approach_speed) {
			std::clog << "plane speed is " << this->speed << " kt <=> " << this->settings.max_approach_speed << " kt" << std::endl;				
			return false;
		}
	}
	
	return true;
}

void Aircraft::calculate_angle_target(Coordinate& target) {
	double t_angle = Tools::angle(this->place, target);
	
	this->clearance_heading = t_angle;
	this->turn = (this->heading < this->clearance_heading) ? 1 : -1;
	
	if (std::abs(this->heading - t_angle) > 0.5) {
		this->heading = t_angle;
	}
}

void Aircraft::update(double elapsed) {
	this->heading = Tools::fix_angle(this->heading);
	
	if (this->speed > 145 && this->type == DEPARTURE && this->altitude < 500) {
		this->set_clearance_altitude(4000);
	} else if (this->type == DEPARTURE && this->altitude > 1500 && this->altitude < this->settings.shortcut) {
		this->set_clearance_speed(250);
	} else if (this->type == DEPARTURE && this->altitude > 10000) {
		this->set_clearance_speed(320);
	}

	if (this->approach) {
		if (this->landed == false) {
			double t_distance = Tools::distanceNM(this->place, this->landing.get_start_place()) * 6076.11549;
			double target_approach_altitude = std::tan(Tools::deg2rad(this->landing.get_glidepath())) * t_distance;
			
			if (!this->check_approach_config()) {
				this->approach = false;
				return;
			}
			
			if (this->altitude - this->settings.airfield_altitude < 1200) {
				this->clearance_speed = 180;
			} 
			
			if (this->altitude - this->settings.airfield_altitude < 800) {
				this->clearance_speed = 160;
			} 
			
			if (this->altitude - this->settings.airfield_altitude < 300) {
				this->clearance_speed = 135;
			}
			
			if (this->altitude > this->settings.airfield_altitude) {
				if (this->altitude > target_approach_altitude) {
					this->altitude = target_approach_altitude;
				}
			} 
			
			if (Tools::on_area(this->place, this->landing.get_start_place())) {
				this->altitude = this->settings.airfield_altitude;
				this->landed = true;
			}  else if (Tools::on_area(this->place, this->landing.get_approach_place()) && this->final_approach == false) {
				this->approach_target = this->landing.get_start_place();
				this->final_approach = true;
			}else {
				this->calculate_angle_target(this->approach_target);
			}
		} else {
			this->clearance_speed = 10;
			this->heading = this->landing.get_heading();
			this->altitude = this->settings.airfield_altitude;
		}
	} else if (this->direct) {
		this->calculate_angle_target(this->target.get_place());
	}
	
	this->altitude  = change_parameter(elapsed, this->altitude, this->clearance_altitude, 	this->settings.altitude_change);
	this->speed     = change_parameter(elapsed, this->speed, 	this->clearance_speed, 		this->settings.speed_change);
	this->heading   = change_parameter(elapsed, this->heading, 	this->clearance_heading, 	Tools::deg2rad(this->settings.heading_change), this->turn);
	
	double distance = this->speed * (elapsed / 1000) / 3600;
	this->place = Tools::calculate(this->place, this->heading, distance);
}

Coordinate& Aircraft::get_place() {
    return this->place;
}

Navpoint& Aircraft::get_target() {
	return this->target;
}

std::string Aircraft::get_name() {
    return this->name;
}

double Aircraft::change_parameter(double elapsed, double actual_value, double clearance_value, double change, int turn) {
	if (clearance_value > 0) {
		elapsed /= 1000.0;
		double add = turn;
		
		if ((int)add == 0) {
			add = (clearance_value > actual_value) ? 1.0 : -1.0;
		}
		
		if (std::abs(actual_value - clearance_value) < (0.01 * clearance_value)) {
			return clearance_value;
		} else {
			actual_value += elapsed * change * add;
		}
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

void Aircraft::set_place(Coordinate& place) {
    this->place = place;
}

void Aircraft::set_clearance_speed(double cl_spd) {
	this->clearance_speed = cl_spd;
}

void Aircraft::set_clearance_heading(double cl_hdg, int turn) {
	this->clearance_heading = cl_hdg;
	
	this->clearance_heading = Tools::fix_angle(this->clearance_heading);
	
	this->turn = turn;
}

void Aircraft::set_clearance_altitude(double cl_alt) {
	this->clearance_altitude = cl_alt;
}

void Aircraft::set_clearance_approach() {
	this->approach = true;
	this->approach_target = this->landing.get_approach_place();
}

void Aircraft::cancel_approach() {
	this->approach = false;
}

void Aircraft::set_clearance_direct(std::string outpoint) {
	try {
		Outpoint t_op = this->airport->get_outpoint(outpoint);
		this->target = t_op;
	this->direct = true;
	} catch (std::logic_error& e) {
		std::cerr << e.what() << std::endl;
	}
}

double Aircraft::get_clearance_altitude() {
	return (this->clearance_altitude < 0) ? 0 : this->clearance_altitude;
}

double Aircraft::get_clearance_speed() {
	return (this->clearance_speed < 0) ? 0 : this->clearance_speed;
}

double Aircraft::get_clearance_heading() {
	return (this->clearance_heading < 0) ? 0 : this->clearance_heading;
}

bool Aircraft::get_direct() {
	return this->direct;
}

bool Aircraft::get_approach() {
	return this->approach;
}

bool Aircraft::get_expect() {
	return this->expect;
}

std::string Aircraft::get_landing_runway_name() {
	return this->landing.get_name();
}

std::string Aircraft::get_target_name() {
	return this->target.get_name();
}
