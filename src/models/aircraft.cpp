#include "aircraft.hpp"

Aircraft::Aircraft(std::string t_name, Settings& s, Airfield*& af, Atis*& a, Inpoint& ip) : name(t_name), settings(s), airport(af), atis(a) {
	this->place 				= ip.get_place();
	this->heading 				= ip.get_heading();
	this->altitude 				= ip.get_altitude();
	this->speed 				= ip.get_speed();
	this->type 					= APPROACH;
	
	this->load();
}

Aircraft::Aircraft(std::string t_name, Settings& s, Airfield*& af, Atis*& a, Outpoint& op) : name(t_name), settings(s), airport(af), atis(a) {	
	this->place 				= this->airport->get_runway(this->atis->get_departure_runway()).get_start_place();
	this->altitude 				= this->settings.airfield_altitude;
	this->heading 				= this->airport->get_runway(this->atis->get_departure_runway()).get_heading();
	this->speed					= 0;

	this->target 				= op;
	this->type 					= DEPARTURE;
	
	this->load();
}

void Aircraft::load() {
	this->heading = Tools::fix_angle(this->heading);
    this->separation_error 		= false;
    this->approach 				= false;
    this->landed 				= false;
    this->direct 				= false;
	this->expect 				= false;
	this->final_approach 		= false;
	
	this->clearance_altitude 	= this->altitude;
    this->clearance_speed 		= this->speed;
	this->clearance_heading 	= this->heading;
	
    this->turn 					= LEFT;
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
	bool heading_ok = true;
	bool speed_ok = true;
	bool altitude_ok = true;
	
	double min_approach_angle = Tools::fix_angle(this->landing.get_heading() - Tools::deg2rad(this->settings.approach_angle));
	double max_approach_angle = Tools::fix_angle(this->landing.get_heading() + Tools::deg2rad(this->settings.approach_angle));
	
	std::clog << "Heading must be between " << min_approach_angle << " and " << max_approach_angle << std::endl;
	std::clog << "Plane heading is " << this->heading << std::endl;
	
	if (this->altitude > this->settings.max_approach_altitude) {
		std::clog << "plane altitude is " << this->altitude << " ft " << this->settings.max_approach_altitude << " ft" << std::endl;
		
		altitude_ok = false;
	}
	
	if (this->heading > max_approach_angle || this->heading	< min_approach_angle) {
		std::clog << "max approach angle " << Tools::rad2deg(max_approach_angle) << " < " << Tools::rad2deg(this->heading) << std::endl;
		std::clog << "min approach angle " << Tools::rad2deg(min_approach_angle) << " < " << Tools::rad2deg(this->heading) << std::endl;
		
		heading_ok = false;
	} 
	
	if (this->speed > this->settings.max_approach_speed) {
		std::clog << "plane speed is " << this->speed << " kt <=> " << this->settings.max_approach_speed << " kt" << std::endl;				
		speed_ok = false;
	}
	
	return (speed_ok && altitude_ok && heading_ok);
}

void Aircraft::calculate_angle_target(Coordinate& target) {
	double t_angle = Tools::angle(this->place, target);
	
	this->clearance_heading = t_angle;
	this->turn = (this->heading < this->clearance_heading) ? RIGHT : LEFT;
}

void Aircraft::update(double elapsed) {
	this->heading = Tools::fix_angle(this->heading);
	
	if (this->type == DEPARTURE && this->speed > 145 && this->altitude < 500) {
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
			} else {
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
	
	change_altitude(elapsed);
	change_speed(elapsed);
	change_heading(elapsed);
	
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

void Aircraft::change_speed(double elapsed) {
	if (this->clearance_speed > 0) {
		int multiply = (this->clearance_speed < this->speed) ? DECREASE : INCREASE;
		
		if (std::abs(this->speed - this->clearance_speed) < (0.01 * this->clearance_speed)) {
			this->speed = this->clearance_speed;
		} else {
			this->speed += (elapsed / 1000) * this->settings.speed_change * multiply;
		}
	}
}

void Aircraft::change_altitude(double elapsed) {
	if (this->clearance_altitude > 0) {
		int multiply = (this->clearance_altitude < this->altitude) ? DESCENT : CLIMB;
		
		if (std::abs(this->altitude - this->clearance_altitude) < (0.01 * this->clearance_altitude)) {
			this->altitude = this->clearance_altitude;
		} else {
			this->altitude += (elapsed / 1000) * this->settings.altitude_change * multiply;
		}
	}
}

void Aircraft::change_heading(double elapsed) {
	if (this->clearance_heading != 0) {
		if (std::abs(this->heading - this->clearance_heading) < (0.01 * this->clearance_heading)) {
			this->heading = this->clearance_heading;
		} else {
			this->heading += (elapsed / 1000) * Tools::deg2rad(this->settings.heading_change) * this->turn;
		}
	}
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
	this->clearance_heading = Tools::fix_angle(cl_hdg);
	
	this->turn = turn;
}

void Aircraft::set_clearance_altitude(double cl_alt) {
	this->clearance_altitude = cl_alt;
}

void Aircraft::set_clearance_approach() {
	this->approach = true;
	
	if (!this->check_approach_config()) {
		this->approach = false;
		return;
	}
	
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
