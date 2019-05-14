#include "game.hpp"

Game::Game(std::shared_ptr <Settings> s, std::shared_ptr <Atis> a) : settings(s), atis(a) {
	#ifdef DEBUG
    std::clog << "Game::game()" << std::endl;
	#endif
	
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
	#ifdef DEBUG
    std::clog << "Game::get_active_field()" << std::endl;
	#endif
	
	if (this->active_field == NULL) {
		throw std::logic_error("Game::get_active_field() this->active_field == NULL");
	}
	
    return this->active_field;
}

std::list <aircraft> Game::get_aircrafts() {
	#ifdef DEBUG
    std::clog << "Game::get_aircrafts()" << std::endl;
	#endif
	
    return this->aircrafts;
}

void Game::add_point(Navpoint np) {
	#ifdef DEBUG
    std::clog << "Game::add_point())" << std::endl;
	#endif

    this->navpoints.push_back(np);
}

void Game::check_collision() {
	#ifdef DEBUG
    std::clog << "Game::check_collision()" << std::endl;
	#endif
	
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
	#ifdef DEBUG
    std::clog << "Game::is_free()" << std::endl;
	#endif
	
    std::list <aircraft> :: iterator plane;

    for (plane = this->aircrafts.begin(); plane != this->aircrafts.end(); ++plane) {
        if ((*plane)->get_type() == APPROACH) {
			#ifdef DEBUG
			std::clog << "APPROACH" << std::endl;
			#endif
			if (Tools::distanceNM(navpoint.get_place(), (*plane)->get_place()) < this->settings->separation_horizontal) { 
				#ifdef DEBUG
				std::clog << navpoint.get_place() << " vs " << (*plane)->get_place() << "is less than required " << this->settings->separation_horizontal << std::endl;
				#endif
				if (std::abs((*plane)->get_altitude() - navpoint.get_altitude()) < this->settings->separation_vertical) {
					#ifdef DEBUG
					std::clog << (*plane)->get_altitude() << " vs " << navpoint.get_altitude() << "is less than required " << this->settings->separation_vertical << std::endl;
					#endif
					return false;
				} else {
					#ifdef DEBUG
					std::clog << (*plane)->get_altitude() << " vs " << navpoint.get_altitude() << "is more than required " << this->settings->separation_vertical << std::endl;
					#endif
				}
			} else {
				#ifdef DEBUG
				std::clog << navpoint.get_place() << " vs " << (*plane)->get_place() << "is more than required " << this->settings->separation_horizontal << std::endl;
				#endif
			}
        } else {
			#ifdef DEBUG
			std::clog << "DERPARTURE" << std::endl;
			#endif
		}
    }

    return true;
}

void Game::handle_holdings() {
	#ifdef DEBUG
    std::clog << "Game::handle_holdings()" << std::endl;
	#endif
	
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
        this->holdings.pop_front();
		this->pop_holdings += this->settings->departure_separation * 1000 * 60;
	}
}

void Game::calculate_points(int type, double clearance_count, std::string plane) {
	#ifdef DEBUG
    std::clog << "Game::calculate_points(" << type << ", " << clearance_count << ", " << plane << ")" << std::endl;
	#endif
	
	this->points[plane].out_time = duration;
	this->points[plane].clearances = clearance_count;
	
	++this->handled_planes;
	
	double point = (type == APPROACH) ? 8000 : 4000;
	
	double area_time = this->points[plane].out_time - this->points[plane].in_time;
	
	point = (int)(point / clearance_count) - (int)(area_time / 1000);
	
	this->points[plane].points = point;
	this->points[plane].area_time = area_time;
}

std::map <std::string, Game_point> Game::get_points() {
	#ifdef DEBUG
    std::clog << "Game::get_points())" << std::endl;
	#endif
	
	return this->points;
}

double Game::get_game_points() {
	#ifdef DEBUG
    std::clog << "Game::get_game_points()" << std::endl;
	#endif
	
	std::map <std::string, Game_point> :: iterator pit = this->points.begin();
	double sum = 0;
	
	while (pit != this->points.end()) {
		sum += pit->second.points;
		
		++pit;
	}
	
	return sum;
}

void Game::create_planes(int amount) {
	#ifdef DEBUG
    std::clog << "Game::create_planes(" << amount << ")" << std::endl;
	#endif

	for (int i = 0; i < amount; ++i) {
		this->create_plane();
	}
}

void Game::update(double elapsed) {	
	#ifdef DEBUG
    std::clog << "Game::get_game_points(" << elapsed << ")" << std::endl;
	#endif
	
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
	#ifdef DEBUG
    std::clog << "Game::get_duration()" << std::endl;
	#endif
	
    return this->duration;
}

Inpoint Game::select_inpoint() {
	#ifdef DEBUG
    std::clog << "Game::select_inpoint()" << std::endl;
	#endif
	
	Inpoint inpoint = Tools::random_object <Inpoint> (this->active_field->get_inpoints());
	
	
	while (!is_free(inpoint)) {
		inpoint = Tools::random_object <Inpoint> (this->active_field->get_inpoints());
	}
	
	#ifdef DEBUG
	std::clog << inpoint.get_name() << std::endl;
	#endif
	
	return inpoint;
}


Outpoint Game::select_outpoint() {
	#ifdef DEBUG
    std::clog << "Game::select_outpoint()" << std::endl;
	#endif
	
	Outpoint outpoint = Tools::random_object <Outpoint> (this->active_field->get_outpoints());
	
	#ifdef DEBUG
	std::clog << outpoint.get_name() << std::endl;
	#endif
	
	return outpoint;
}

bool Game::check_aircrafts(std::string name) {
	#ifdef DEBUG
    std::clog << "Game::check_aircrafts()" << std::endl;
	#endif
	
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

	Inpoint inpoint = this->select_inpoint();
	Outpoint outpoint = this->select_outpoint();
	
	Queryresult airlines = db.get_result("SELECT ICAO FROM airlines");
	
	int t_type = Tools::linear_random(1, 100);
	
	types flight_type = (t_type > 50) ? APPROACH : DEPARTURE;

    std::string callsign = airlines(Tools::linear_random(0, airlines.size()), "ICAO") + Tools::tostr(Tools::linear_random(1, 999), 3);

	Game_point tmp{0, duration, -1, 0, -1};
	
	this->points.insert(std::pair <std::string, Game_point> (callsign, tmp));

	while (!this->check_aircrafts(callsign)) {
		callsign = airlines(Tools::linear_random(0, airlines.size()), "ICAO") + Tools::tostr(Tools::linear_random(1, 999), 3);
	}
	
	if (flight_type == DEPARTURE) {
		aircraft plane(new Aircraft(callsign, this->settings, this->active_field, this->atis, outpoint));
		this->holdings.push_back(plane);
	} else {
		aircraft plane (new Aircraft(callsign, this->settings, this->active_field, this->atis, inpoint));
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
	#ifdef DEBUG
    std::clog << "Game::get_sekected()" << std::endl;
	#endif
	
    return this->selected;
}

std::list <Clearance> Game::get_clearances() {
	#ifdef DEBUG
    std::clog << "Game::get_clearances()" << std::endl;
	#endif
	
	return this->clearances;
}

int Game::calculate_clearances(std::string name) {
	#ifdef DEBUG
    std::clog << "Game::calculate_clearances()" << std::endl;
	#endif
	
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
	#ifdef DEBUG
    std::clog << "Game::build_clearnace(" << command << ")" << std::endl;
	#endif
	
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
	#ifdef DEBUG
    std::clog << "Game::get_message()" << std::endl;
	#endif
	
	std::string front;
	
	if (!this->display_messages.empty()) {
		front = this->display_messages.front();
		
		this->display_messages.pop();
		
		return front;
	} 
	
	return "";
}

void Game::clear_errors() {
	#ifdef DEBUG
    std::clog << "Game::clear_errors()" << std::endl;
	#endif
	
	while (!this->display_messages.empty()) {
		this->display_messages.pop();
	}
}

int Game::get_separation_errors() {
	#ifdef DEBUG
    std::clog << "Game::get_separation_errors()" << std::endl;
	#endif
	
	return this->separation_errors;
}

std::string Game::get_clearance_error() {
	#ifdef DEBUG
    std::clog << "Game::get_clearance_error()" << std::endl;
	#endif
	
	if (display_messages.size()) {
		return display_messages.front();
	}
	
	return "";
}

void Game::remove_first_game_error() {
	#ifdef DEBUG
    std::clog << "Game::remove_first_game_error()" << std::endl;
	#endif
	
	if (game_errors.size()) {
		game_errors.pop();
	}
}

std::string Game::get_game_error() {
	#ifdef DEBUG
    std::clog << "Game::get_game_error()" << std::endl;
	#endif
	
	if (game_errors.size()) {
		return game_errors.front();
	}
	
	return "";
}

int Game::get_handled_planes() {
	#ifdef DEBUG
    std::clog << "Game::get_handled_planes()" << std::endl;
	#endif
	
	return this->handled_planes;
}

int Game::get_planes_count() {
	#ifdef DEBUG
    std::clog << "Game::get_planes_count()" << std::endl;
	#endif
	
	return this->aircrafts.size();
}

int Game::get_new_plane() {
	#ifdef DEBUG
    std::clog << "Game::get_new_plane()" << std::endl;
	#endif
	
	return this->new_plane;
}

bool Game::ok() {
	#ifdef DEBUG
    std::clog << "Game::ok()" << std::endl;
	#endif
	
	return (this->handled_planes == this->settings->required_handled && this->aircrafts.size() == 0);
}

int Game::get_level() {
	#ifdef DEBUG
    std::clog << "Game::get_level()" << std::endl;
	#endif
	
	return this->level;
}

void Game::set_level(int level) {
	#ifdef DEBUG
    std::clog << "Game::set_level(" << level << ")" << std::endl;
	#endif
	
	this->level = level;
}

std::list <aircraft> Game::get_holdings() {
	return this->holdings;
}