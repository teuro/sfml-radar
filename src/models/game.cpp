#include "game.hpp"

Game::Game(std::shared_ptr <Settings> s, std::shared_ptr <Atis> a) : settings(s), atis(a) {
    this->duration = 0;
	this->selected = NULL;
	this->loaded = false;
}

Game::~Game() { }

std::shared_ptr <Atis> Game::get_atis() {
	return this->atis;
}

void Game::load(std::string airfield) {
	#ifdef DEBUG
    std::clog << "Game::load(" << airfield << ")" << std::endl;
	#endif
	
    this->load_airfield(airfield);

    this->duration = 0;
	
	this->separation_errors = 0;
    this->new_plane = 5000;
    this->pop_holdings = 500;
	this->handled_planes = 0;
	
	this->loaded = true;
}

std::shared_ptr <Airfield> Game::get_active_field() {
	if (this->active_field == NULL) {
		throw std::logic_error("Game::get_active_field() this->active_field == NULL");
	}
	
    return this->active_field;
}

std::list <aircraft> Game::get_aircrafts() {
    return this->aircrafts;
}

void Game::add_point(Navpoint np) {
    this->navpoints.push_back(np);
}

void Game::check_collision() {
    this->errors.clear();

    std::list <aircraft> :: iterator plane_a;
    std::list <aircraft> :: iterator plane_b;

    for (plane_a = this->aircrafts.begin(); plane_a != this->aircrafts.end(); ++plane_a) {
        for (plane_b = this->aircrafts.begin(); plane_b != this->aircrafts.end(); ++plane_b) {
            if (*plane_a == *plane_b) {
                continue;
            }

            if ((*plane_a)->get_altitude() >= 1000 && (*plane_b)->get_altitude() >= 1000 && std::abs((*plane_a)->get_altitude() - (*plane_b)->get_altitude()) < this->settings->separation_vertical) {
                if (Tools::distanceNM((*plane_a)->get_place(), (*plane_b)->get_place()) < this->settings->separation_horizontal) {
                    this->errors.push_back((*plane_a));
                    this->errors.push_back((*plane_b));

                    if (!(*plane_a)->get_separation_error() || !(*plane_b)->get_separation_error()) {
                        game_errors.push("Separation error occured between " + (*plane_a)->get_name() + " and " + (*plane_b)->get_name());
                        ++this->separation_errors;
                    }
                }
            }
        }
    }
}

bool Game::is_free(Inpoint& navpoint) {
    std::list <aircraft> :: iterator plane;

    for (plane = this->aircrafts.begin(); plane != this->aircrafts.end(); ++plane) {
        if (Tools::distanceNM(navpoint.get_place(), (*plane)->get_place()) < this->settings->separation_horizontal && std::abs((*plane)->get_altitude() - navpoint.get_altitude()) < this->settings->separation_vertical) {
            return false;
        }
    }

    return true;
}

void Game::handle_holdings() {
    if (this->holdings.size()) {
        std::list <aircraft> :: iterator plane = this->aircrafts.begin();

        while (plane != this->aircrafts.end()) {
            if ((*plane)->get_altitude() < 1000 || this->duration < this->pop_holdings) {
                return;
            }

            ++plane;
        }

        aircraft t = this->holdings.front();
        t->set_place(this->atis->get_departure_runway().get_start_place());
        t->set_takeoff_clearance();
		this->aircrafts.push_back(t);
        this->holdings.pop();
		this->pop_holdings += this->settings->departure_separation * 1000 * 60;
	}
}

void Game::calculate_points(int type, double clearance_count, std::string plane) {
	this->points[plane].out_time = duration;
	this->points[plane].clearances = clearance_count;
	
	++this->handled_planes;
	
	double point = (type == APPROACH) ? 8000 : 4000;
	
	double area_time = this->points[plane].out_time - this->points[plane].in_time;
	
	point = (int)(point / clearance_count) - (int)area_time;
	
	this->points[plane].points = point;
}

std::map <std::string, Game_point> Game::get_points() {
	return this->points;
}

double Game::get_game_points() {
	std::map <std::string, Game_point> :: iterator pit = this->points.begin();
	double sum = 0;
	
	while (pit != this->points.end()) {
		sum += pit->second.points;
		
		++pit;
	}
	
	return sum;
}

void Game::create_planes(int amount) {
	for (int i = 0; i < amount; ++i) {
		this->create_plane();
	}
}

void Game::update(double elapsed) {	
	if (!this->loaded) {
		throw std::logic_error("Game is not loaded");
	}
	
	this->duration += elapsed;
    this->check_collision();
    this->handle_holdings();

    for (std::list <aircraft> :: iterator it = this->aircrafts.begin(); it != this->aircrafts.end(); ++it) {
		std::string aircraft_message;
		
        (*it)->set_separation_error(false);
        aircraft_message = (*it)->update(elapsed);
		
		if ((*it)->get_speed() < 20 && (*it)->get_type() == APPROACH) {
			calculate_points(APPROACH, this->calculate_clearances((*it)->get_name()), (*it)->get_name());
			it = this->aircrafts.erase(it);
			continue;
		} 
		
		if (Tools::on_area((*it)->get_place(), (*it)->get_target().get_place()) && (*it)->get_type() == DEPARTURE) {
			calculate_points(DEPARTURE, this->calculate_clearances((*it)->get_name()), (*it)->get_name());
			it = this->aircrafts.erase(it);
			continue;
		}
    }

    for (std::list <aircraft> :: iterator it = errors.begin(); it != errors.end(); ++it) {
        (*it)->set_separation_error(true);
    }

    if (this->duration > this->new_plane && this->settings->required_handled > this->handled_planes + this->aircrafts.size() + this->holdings.size()) {
        create_plane();
        double time_for_next_plane = Tools::linear_random(this->settings->new_plane_lower * 1000, this->settings->new_plane_upper * 1000);
		this->new_plane += time_for_next_plane;
    }
}

double Game::get_duration() {
    return this->duration;
}

Inpoint Game::select_inpoint() {
	Inpoint t_inpoint = Tools::random_object<Inpoint>(this->active_field->get_inpoints());
	
	return t_inpoint;
}

bool Game::check_aircrafts(std::string name) {
	std::list <aircraft> :: iterator plane;

    for (plane = this->aircrafts.begin(); plane != this->aircrafts.end(); ++plane) {
		if ((*plane)->get_name() == name) {
			return false;
		}
	}
	
	return true;
}

void Game::create_plane() {
	#ifdef DEBUG
	std::clog << "Game::create_plane()" << std::endl;
	#endif
	
	Database db;
	
	if (this->active_field == NULL) {
		throw std::logic_error("Airfield not ready");
	}

	Inpoint t_inpoint = this->select_inpoint();
	
	while (!is_free(t_inpoint)) { 
		t_inpoint = this->select_inpoint();
	}
	
	Outpoint t_outpoint = Tools::random_object<Outpoint>(this->active_field->get_outpoints());
	
	Queryresult airlines = db.get_result("SELECT ICAO FROM airlines");
	
	int t_type = Tools::linear_random(1, 100);
	
	/** 
	testing data
	double heading = Tools::deg2rad(227);
	Coordinate test(60.3680, 25.0666);
	t_type = 51; 
	**/
	
	types type = (t_type < 50) ? DEPARTURE : APPROACH;

    std::string t_callsign;
	t_callsign = airlines(Tools::linear_random(0, airlines.size()), "ICAO") + Tools::tostr(Tools::linear_random(1, 999), 3);

	Game_point tmp{0, duration, -1, 0};
	this->points.insert(std::pair <std::string, Game_point> (t_callsign, tmp));

	while (!this->check_aircrafts(t_callsign)) {
		t_callsign = airlines(Tools::linear_random(0, airlines.size()), "ICAO") + Tools::tostr(Tools::linear_random(1, 999), 3);
	}
	
	if (type == DEPARTURE) {
		aircraft plane(new Aircraft(t_callsign, this->settings, this->active_field, this->atis, t_outpoint));
		this->holdings.push(plane);
	} else {
		aircraft plane (new Aircraft(t_callsign, this->settings, this->active_field, this->atis, t_inpoint));
		this->aircrafts.push_back(plane);
	}
}

void Game::load_navpoints(int airfield_id) {
	#ifdef DEBUG
	std::clog << "Game::load_navpoints(" << airfield_id << ")" << std::endl;
	#endif
	
	std::list <std::string> variables;
	
	variables.push_back(Tools::tostr(airfield_id));
	
	Database db;
	
	try {
		std::string query = db.bind_param("SELECT name, latitude, longitude, altitude, heading, type FROM navpoints WHERE airfield_id = ?", variables);
		
		Queryresult q_navpoints = db.get_result(query);
		
		for (unsigned int i = 0; i < q_navpoints.size(); ++i) {
			double t_lat = Tools::tonumber<double>(q_navpoints(i, "latitude"));
			double t_lon = Tools::tonumber<double>(q_navpoints(i, "longitude"));

			Coordinate t_place(t_lat, t_lon);
			std::string t_name  = q_navpoints(i, "name");
			int type = Tools::toint(q_navpoints(i, "type"));
			
			if (type == 2) {
				double t_altitude   = Tools::tonumber<double>(q_navpoints(i, "altitude"));
				double t_heading    = Tools::tonumber<double>(q_navpoints(i, "heading"));

				this->active_field->add_inpoint(Inpoint(t_name, t_place, 250, t_altitude, Tools::deg2rad(t_heading)));
			} else {
				this->active_field->add_outpoint(Outpoint(t_name, t_place));
			}
		}
	} catch ( ... ) {
		throw;
	}
}

void Game::load_runways(int airfield_id) {
	#ifdef DEBUG
	std::clog << "Game::load_runways(" << airfield_id << ")" << std::endl;
	#endif
	
	std::list <std::string> variables;
	
	variables.push_back(Tools::tostr(airfield_id));
	
	Database db;
	
	try {
		std::string query = db.bind_param("SELECT name, start_latitude, start_longitude, end_latitude, end_longitude FROM runways WHERE airfield_id = ?", variables);
		
		Queryresult q_runways = db.get_result(query);

		for (unsigned int i = 0; i < q_runways.size(); ++i) {
			double s_lat = Tools::tonumber<double>(q_runways(i, "start_latitude"));
			double s_lon = Tools::tonumber<double>(q_runways(i, "start_longitude"));
			double e_lat = Tools::tonumber<double>(q_runways(i, "end_latitude"));
			double e_lon = Tools::tonumber<double>(q_runways(i, "end_longitude"));
		
			Coordinate start_p(s_lat, s_lon);
			Coordinate end_p(e_lat, e_lon);
			double angle = Tools::fix_angle(Tools::angle(start_p, end_p) - Tools::get_PI());
			Coordinate approach = Tools::calculate(start_p, angle, this->settings->approach_point_distance);

			Runway rwy(q_runways(i, "name"), start_p, end_p, approach);
			
			this->active_field->add_runway(rwy);
		}
	} catch ( ... ) {
		throw;
	}
}

void Game::load_airfield(std::string icao) {
	#ifdef DEBUG
	std::clog << "Game::load_airfield(" << icao << ")" << std::endl;
	#endif
	
	std::list <std::string> t_airport;
	t_airport.push_back(icao);
	
	Database db;
	
    Queryresult airport = db.get_result(db.bind_param("SELECT ROWID AS airfield_id, ICAO, latitude, longitude, altitude, max_speed, initial_altitude, acceleration_altitude FROM airfields WHERE ICAO = '?'", t_airport));
	
	int airfield_id = Tools::toint(airport(0, "airfield_id"));

    Coordinate place(Tools::tonumber<double>(airport(0, "latitude")), Tools::tonumber<double>(airport(0, "longitude")));
	
	std::shared_ptr <Airfield> ap (new Airfield(airport(0, "ICAO"), place, Tools::toint(airport(0, "max_speed")), Tools::toint(airport(0, "initial_altitude")), Tools::toint(airport(0, "acceleration_altitude"))));
    this->active_field = ap;
	
	#ifdef DEBUG
	std::clog << "Game::load_airfield(" << icao << ")" << this->active_field->get_place() << std::endl;
	#endif
	
	this->settings->airfield_altitude = Tools::toint(airport(0, "altitude"));
	this->settings->set_centerpoint(place);
	
	this->load_navpoints(airfield_id);
	this->load_runways(airfield_id);
	
	#ifdef DEBUG
	std::clog << "airfield loaded ok" << std::endl;
	#endif
}

aircraft Game::get_selected() {
    return this->selected;
}

std::list <Clearance> Game::get_clearances() {
	return this->clearances;
}

int Game::calculate_clearances(std::string name) {
	int count = 0;
	
	std::list <Clearance> :: iterator it = this->clearances.begin();
	
	while (it != this->clearances.end()) {
		if ((*it).plane == name) {
			++count;
		}
		++it;
	}
	
	return count;
}

void Game::build_clearance(std::string command) {
	std::vector <std::string> tmp = Tools::split(" ", command);
	
	if (this->selected != NULL) {
		Clearance t_clearance = {this->duration, this->selected->get_name(), command};
		this->clearances.push_back(t_clearance);
		
		std::string s_value = Tools::trim(tmp.back());
		int value;
		
		if (tmp.size() > 1) {
			if (Tools::trim(tmp[0]) == "turn") {
				value = Tools::toint(s_value);
				
				int turn = LEFT;
				
				if (Tools::trim(tmp[1]) == "right") { 
					turn = RIGHT; 
				} else if (Tools::trim(tmp[1]) == "left") {
					turn = LEFT;
				} else if (tmp.size() > 2) {
					turn = ERROR;
				}
				
				if (turn == LEFT || turn == RIGHT) {
					this->selected->set_clearance_heading(Tools::deg2rad(value), turn);
				} else {
					display_messages.push("turn must be either 'left' or 'right'");
				}
			} else if (Tools::trim(tmp[0]) == "climb") {
				value = Tools::toint(s_value);
				if (this->selected->get_altitude() > value) {
					display_messages.push("Can't climb, because altitude " + Tools::tostr(this->selected->get_altitude()) + " ft is higher than " + Tools::tostr(value) + " ft");
				} else {
					this->selected->set_clearance_altitude(value);
				}
			} else if (Tools::trim(tmp[0]) == "descent") {
				value = Tools::toint(s_value);
				if (this->selected->get_altitude() < value) {
					display_messages.push("Can't descent, because altitude " + Tools::tostr(this->selected->get_altitude()) + " ft is lower than " + Tools::tostr(value) + " ft");
				} else {
					this->selected->set_clearance_altitude(value);
				}
			} else if (Tools::trim(tmp[0]) == "speed") {
				value = Tools::toint(s_value);
				if (value > this->settings->clearance_speed_upper) {
					display_messages.push("Maximum clearance speed is " + Tools::tostr(this->settings->clearance_speed_upper) + " knots");
				} else if (value < this->settings->clearance_speed_lower) {
					display_messages.push("Minimum clearance speed is " + Tools::tostr(this->settings->clearance_speed_upper) + " knots");
				} else { 
					this->selected->set_clearance_speed(value);
				}
			} else if (Tools::trim(tmp[0]) == "expect") { 
				if (this->selected->get_type() == APPROACH) {
					std::string landing = Tools::trim(s_value);
					display_messages.push(this->selected->set_approach_runway(landing));
				} else {
					display_messages.push("Plane must be approaching not departing");
				}
			} else if (Tools::trim(tmp[0]) == "direct") {
				if (this->selected->get_altitude() < this->settings->shortcut) {
					display_messages.push("Unable to comply because altitude must be greater than " + Tools::tostr(this->settings->shortcut) + " ft");
				} else {
					if (this->selected->get_type() == DEPARTURE) {
						this->selected->set_clearance_direct(s_value);
					} else {
						display_messages.push("Plane must be departing not approaching");
					}
				}
			}
		} else if (Tools::trim(tmp[0]) == "approach") {
			if (this->selected->get_type() == APPROACH) {
				std::string tmp_msg = this->selected->set_clearance_approach();
				display_messages.push(tmp_msg);
			} else {
				display_messages.push("Plane must be approaching not departing");
			}
		} else if (Tools::trim(tmp[0]) == "cancel") {
			if (this->selected->get_type() == APPROACH) {
				this->selected->cancel_approach();
			} else {
				display_messages.push("Plane must be approaching not departing");
			}
		} else {
			display_messages.push("Unknown command");
		} 
	} else {
		display_messages.push("No selected plane");
	}
}

std::string Game::get_message() {
	std::string front;
	
	if (!this->display_messages.empty()) {
		front = this->display_messages.front();
		
		this->display_messages.pop();
		
		return front;
	} 
	
	return "";
}

void Game::clear_errors() {
	while (!this->display_messages.empty()) {
		this->display_messages.pop();
	}
}

int Game::get_separation_errors() {
	return this->separation_errors;
}

void Game::remove_first_clearance_error() {
	if (display_messages.size()) {
		display_messages.pop();
	}
}

std::string Game::get_clearance_error() {
	if (display_messages.size()) {
		return display_messages.front();
	}
	
	return "";
}

void Game::remove_first_game_error() {
	if (game_errors.size()) {
		game_errors.pop();
	}
}

std::string Game::get_game_error() {
	if (game_errors.size()) {
		return game_errors.front();
	}
	
	return "";
}

int Game::get_handled_planes() {
	return this->handled_planes;
}

int Game::get_planes_count() {
	return this->aircrafts.size();
}

int Game::get_new_plane() {
	return this->new_plane;
}

bool Game::ok() {
	return (this->handled_planes == this->settings->required_handled);
}

int Game::get_level() {
	return this->level;
}

void Game::set_level(int level) {
	this->level = level;
}