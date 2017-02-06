#include "aircraft.hpp"

Aircraft::Aircraft(std::string name, double speed, double heading, double altitude, Coordinate p, int type, Settings& s, Runway& land, Navpoint& out) : place(p), settings(s), target(out), landing(land) {
    this->name = name;
    this->heading = heading;
    this->altitude = altitude;
    this->speed = speed;

    this->clearance_altitude = -1;
    this->clearance_heading = -1;
    this->clearance_speed = -1;

    this->separation_error = false;
    this->type = type;
    this->approach = false;
    this->direct = false;
    this->turn = -1;
}

Aircraft::~Aircraft() { }

bool Aircraft::check_approach_config(double target_altitude) {
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
	
	if (this->approach) {
		if (this->landed == false) {
			/** Distance from planes position to runway in feets **/
			double t_distance = Tools::distanceNM(this->place, this->landing.get_start_place()) * 6076.11549;
			double target_approach_altitude = std::tan(Tools::deg2rad(this->landing.get_glidepath())) * t_distance;
			
			//std::clog << t_distance << " ft " << target_approach_altitude << " ft " << std::endl;
			
			if (!this->check_approach_config(target_approach_altitude)) {
				this->approach = false;
				return;
			}
			
			if (this->altitude > this->settings.airfield_altitude) {
				if (this->altitude > target_approach_altitude) {
					this->altitude = target_approach_altitude;
				}
			} else {
				this->altitude = this->settings.airfield_altitude;
				this->landed = true;
			}
			
			if (this->altitude - this->settings.airfield_altitude < 1000) {
				this->clearance_speed = 150;
			} 
			
			if (this->altitude - this->settings.airfield_altitude < 500) {
				this->clearance_speed = 135;
			}
			
			this->calculate_angle_target(this->landing.get_start_place());
		} else {
			this->clearance_speed = 0;
			this->heading = this->landing.get_heading();
			this->altitude = this->settings.airfield_altitude;
		}
	} else if (this->direct) {
		this->calculate_angle_target(this->target.get_place());
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
	std::string t_turn = (turn == -1) ? "left" : "right";
	
	this->clearance_heading = Tools::fix_angle(this->clearance_heading);
	
	this->turn = turn;
}

void Aircraft::set_clearance_altitude(double cl_alt) {
	this->clearance_altitude = cl_alt;
}

void Aircraft::set_clearance_approach() {
	this->approach = true;
}

void Aircraft::set_clearance_direct() {
	this->direct = true;
}