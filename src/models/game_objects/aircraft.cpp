#include "aircraft.hpp"

Aircraft::Aircraft(std::string t_name, std::shared_ptr <Settings> s, std::shared_ptr <Airfield> af, std::shared_ptr <Atis> a, Inpoint& ip) : name(t_name), settings(s), airport(af), atis(a) {
	this->place 				= ip.get_place();
	this->heading 				= ip.get_heading();
	this->altitude 				= ip.get_altitude();
	this->speed 				= ip.get_speed();
	this->type 					= APPROACH;
	this->clearances			= 0;
	
	this->vapp					= Tools::linear_random(175, 180);
	this->vland					= Tools::linear_random(130, 138);
	
	this->load();
}

Aircraft::Aircraft(std::string t_name, std::shared_ptr <Settings> s, std::shared_ptr <Airfield> af, std::shared_ptr <Atis> a, Outpoint& op) : name(t_name), settings(s), airport(af), atis(a) {	
	this->place 				= this->atis->get_departure_runway().get_start_place();

	#ifdef DEBUG
	std::clog << place << std::endl;
	#endif

	this->altitude 				= this->airport->get_altitude();
	this->heading 				= this->atis->get_departure_runway().get_heading();
	this->speed					= 0;

	this->target 				= op;
	this->type 					= DEPARTURE;
	this->clearances			= 0;
	
	this->v1					= Tools::linear_random(130, 142);
	this->vr					= Tools::linear_random(145, 147);
	this->v2					= Tools::linear_random(165, 170);
	this->vclimb				= Tools::linear_random(175, 186);
	this->vcruise				= Tools::linear_random(310, 335);
	
	this->load();
}

void Aircraft::load() {
	this->heading = Tools::fix_angle(this->heading);
    this->separation_error 		= false;
    this->approach 				= false;
    this->landed 				= false;
    this->direct 				= false;
	this->expect 				= false;
	this->land	 				= false;
	this->final_approach 		= false;
	
	this->clearance_altitude 	= this->altitude;
    this->clearance_speed 		= this->speed;
	this->clearance_heading 	= this->heading;
	
	this->fuel_consumption		= Tools::linear_random(550, 1210);
	this->fuel_amount			= 250 + Tools::linear_random(1200, 2950) + 150;
	this->fuel_final_reserve	= Tools::linear_random(850, 1130);
	
    this->turn 					= LEFT;
}

Aircraft::~Aircraft() { }

std::string Aircraft::set_approach_runway(std::string name) {
	std::string tmp;
	
	try {
		this->landing = this->airport->get_runway(name);
		this->expect = true;
		tmp = "Expect runway " + name;
	} catch (std::logic_error& e) {
		tmp = e.what();
	}
	
	return tmp;
}

bool Aircraft::check_approach_config() {
	bool heading_ok = true;
	bool speed_ok = true;
	bool altitude_ok = true;
	
	double min_approach_angle = Tools::fix_angle(this->landing.get_heading() - Tools::deg2rad(this->settings->approach_angle));
	double max_approach_angle = Tools::fix_angle(this->landing.get_heading() + Tools::deg2rad(this->settings->approach_angle));
		
	this->approach_config_error = "";
	
	if (this->altitude > this->settings->max_approach_altitude) {
		altitude_ok = false;
		this->approach_config_error += " Altitude is incorrect (" + Tools::tostr(this->settings->max_approach_altitude) + ")";
	}
	
	if (this->heading > max_approach_angle || this->heading	< min_approach_angle) {
		heading_ok = false;
		
		this->approach_config_error += " heading is incorrect ";
	} 
	
	if (this->speed > this->settings->max_approach_speed) {
		speed_ok = false;
		
		this->approach_config_error += " speed is incorrect ";
	}
	
	return (speed_ok && altitude_ok && heading_ok);
}

void Aircraft::calculate_angle_target(Coordinate& target) {
	double t_angle = Tools::angle(this->place, target);
	
	this->clearance_heading = Tools::fix_angle(t_angle);
	this->turn = (this->heading < this->clearance_heading) ? RIGHT : LEFT;
}

std::string Aircraft::update(double elapsed) {
	this->heading = Tools::fix_angle(this->heading);
	
	if (this->type == DEPARTURE) {
		if (this->altitude > this->airport->get_speed_restriction_altitude()) {
			this->clearance_speed = this->vcruise;
		} else if (this->altitude >= this->airport->get_initial_altitude()) {
			this->clearance_speed = this->airport->get_max_speed();
		} else if (this->speed >= this->vclimb && this->clearance_altitude <= this->airport->get_initial_altitude()) {
			this->clearance_altitude = this->airport->get_initial_altitude();
		} else if (this->altitude >= this->airport->get_acceleration_altitude()) {
			this->clearance_speed = this->vclimb;
		} else if (this->speed >= this->vr && this->altitude < this->airport->get_acceleration_altitude()) {
			this->clearance_altitude = this->airport->get_acceleration_altitude();
		} 
	}

	if (this->approach) {
		if (this->landed == false) {
			double t_distance = Tools::nm2ft(Tools::distanceNM(this->place, this->landing.get_start_place()));
			double target_approach_altitude = std::tan(Tools::deg2rad(this->landing.get_glidepath())) * t_distance;
			
			if (this->altitude - this->settings->airfield_altitude < 1200) {
				this->clearance_speed = this->vapp;
			} 
			
			if (this->altitude - this->settings->airfield_altitude < 800) {
				this->clearance_speed = this->vapp-7;
			} 
			
			if (this->altitude - this->settings->airfield_altitude < 300) {
				this->clearance_speed = this->vland;
			}
			
			if (this->altitude > this->settings->airfield_altitude) {
				if (this->altitude > target_approach_altitude) {
					this->altitude = target_approach_altitude;
				}
			} 
			
			if (Tools::on_area(this->place, this->landing.get_start_place())) {
				this->altitude = this->settings->airfield_altitude;
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
			this->altitude = this->settings->airfield_altitude;
		}
	} else if (this->direct) {
		this->calculate_angle_target(this->target.get_place());
	}
	
	change_altitude(elapsed);
	change_speed(elapsed);
	change_heading(elapsed);
	
	double distance = this->speed * (elapsed / 1000) / 3600;
	this->place = Tools::calculate(this->place, this->heading, distance);
	
	this->fuel_amount -= this->fuel_consumption * (elapsed / 1000) / 3600;
	
	if (this->fuel_amount < this->fuel_final_reserve) {
		return this->name + " BINGO " + Tools::tostr(this->fuel_amount) + " kg left";
	}
	
	return this->name  + " " + Tools::tostr(this->fuel_amount) + " kg left";
}

void Aircraft::set_takeoff_clearance() {
	this->clearance_speed = this->v2;
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
			this->speed += (elapsed / 1000) * this->settings->speed_change * multiply;
		}
	}
}

void Aircraft::change_altitude(double elapsed) {
	if (this->clearance_altitude > 0) {
		int multiply = (this->clearance_altitude < this->altitude) ? DESCENT : CLIMB;
		
		if (std::abs(this->altitude - this->clearance_altitude) < (0.01 * this->clearance_altitude)) {
			this->altitude = this->clearance_altitude;
		} else {
			this->altitude += (elapsed / 1000) * this->settings->altitude_change * multiply;
		}
	}
}

void Aircraft::change_heading(double elapsed) {
	if (this->clearance_heading != 0) {
		if (std::abs(this->heading - this->clearance_heading) < (0.025 * this->clearance_heading)) {
			this->heading = this->clearance_heading;
		} else {
			this->heading += (elapsed / 1000) * Tools::deg2rad(this->settings->heading_change) * this->turn;
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

void Aircraft::set_clearance(Speed_clearance& cl_spd) {
	this->clearance_speed = cl_spd.get_value();
	
	++clearances;
}

void Aircraft::set_clearance(Heading_clearance& cl_hdg) {
	this->clearance_heading = Tools::fix_angle(cl_hdg.get_value());
	
	this->turn = cl_hdg.get_direction();
	
	++clearances;
}

void Aircraft::set_clearance(Altitude_clearance& cl_alt) {
	this->clearance_altitude = cl_alt.get_value();
	
	++clearances;
}

void Aircraft::set_clearance(Expect_clearance& cl_exp){
	try {
		this->landing = this->airport->get_runway(cl_exp.get_value());
		this->expect = true;
		
		++clearances;
	} catch (std::logic_error& e) {
		std::string tmp = e.what();
		
		std::cerr << "Aircraft::set_clearance()" << tmp << std::endl;
	}
}

std::string Aircraft::set_clearance_approach() {
	if (!this->check_approach_config()) {
		this->approach = false;
		return this->approach_config_error;
	}
	
	this->approach = true;
	
	this->approach_target = this->landing.get_approach_place();
	
	return "Cleared approach " + get_landing_runway_name();
	
	++clearances;
}

void Aircraft::cancel_approach() {
	this->approach = false;
	
	++clearances;
}

void Aircraft::set_clearance_direct(std::string outpoint) {
	try {
		Outpoint t_op = this->airport->get_outpoint(outpoint);
		this->target = t_op;
		this->direct = true;
		
		++clearances;
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

bool Aircraft::get_landing() {
	return this->land;
}

bool Aircraft::get_expect() {
	return this->expect;
}

bool Aircraft::remove() {
	if (this->type == APPROACH) {
		if (this->landed && speed < this->settings->remove_speed) {
			return true;
		}
	} else {
		if (Tools::on_area(this->get_place(), this->target.get_place())) {
			return true;
		}
	}
	
	return false;
}

int Aircraft::get_clearances() {
	return this->clearances;
}

std::string Aircraft::get_landing_runway_name() {
	return this->landing.get_name();
}

std::string Aircraft::get_target_name() {
	return this->target.get_name();
}
