#include "game.hpp"

Game::Game(Settings& s) : settings(s) {
    this->duration = 0;
}

Game::~Game() { }

void Game::load(std::string airfield, std::string t_departure, std::string t_landing) {
    std::clog << "Game::load(" << airfield << ", " << t_departure << ", " << t_landing << ")" << std::endl;

    this->load_airfield(airfield);

    this->duration = 0;
	this->departure = this->active_field->get_runway(t_departure);
	this->landing = this->active_field->get_runway(t_landing);
	
	this->separation_errors = 0;
    this->new_plane = 5000;
	this->handled_planes = 0;
	this->airlines = Database::get_result("SELECT ICAO FROM airlines");
}

void Game::set_centerpoint(Coordinate& cp) {
	this->centerpoint = cp;
}

Coordinate& Game::get_centerpoint() {
    return this->centerpoint;
}

Airfield* Game::get_active_field() {
	if (this->active_field == NULL) {
		throw std::logic_error("Game::get_active_field() this->active_field == NULL");
	}
	
    return this->active_field;
}

std::vector <Navpoint>& Game::get_navpoints() {
    return this->navpoints;
}

std::list <Aircraft*> Game::get_aircrafts() {
    return this->aircrafts;
}

void Game::add_point(Navpoint np) {
    this->navpoints.push_back(np);
}

void Game::check_collision() {
    this->errors.clear();

    std::list <Aircraft*> :: iterator plane_a;
    std::list <Aircraft*> :: iterator plane_b;

    for (plane_a = this->aircrafts.begin(); plane_a != this->aircrafts.end(); ++plane_a) {
        for (plane_b = this->aircrafts.begin(); plane_b != this->aircrafts.end(); ++plane_b) {
            if (*plane_a == *plane_b) {
                continue;
            }

            if (
                (*plane_a)->get_altitude() >= 1000 &&
                (*plane_b)->get_altitude() >= 1000 &&
                std::abs((*plane_a)->get_altitude() - (*plane_b)->get_altitude()) < this->settings.separation_vertical) {
                if (Tools::distanceNM((*plane_a)->get_place(), (*plane_b)->get_place()) < this->settings.separation_horizontal) {
                    this->errors.push_back((*plane_a));
                    this->errors.push_back((*plane_b));

                    if (!(*plane_a)->get_separation_error() || !(*plane_b)->get_separation_error()) {
                        std::clog << "Separation error occured between " << (*plane_a)->get_name() << " and " << (*plane_b)->get_name() << std::endl;
                        ++this->separation_errors;
                    }
                }
            }
        }
    }
}

bool Game::is_free(Inpoint& navpoint) {
    std::list <Aircraft*> :: iterator plane;

    for (plane = this->aircrafts.begin(); plane != this->aircrafts.end(); ++plane) {
        if (
            Tools::distanceNM(navpoint.get_place(), (*plane)->get_place()) < this->settings.separation_horizontal &&
            std::abs((*plane)->get_altitude() - navpoint.get_altitude()) < this->settings.separation_vertical
        ) {
            return false;
        }
    }

    return true;
}

void Game::handle_holdings() {
    if (this->holdings.size()) {
        std::list <Aircraft*> :: iterator plane = this->aircrafts.begin();

        while (plane != this->aircrafts.end()) {
            if ((*plane)->get_altitude() < 1000) {
                return;
            }

            ++plane;
        }

        Aircraft* t = this->holdings.front();
        t->set_place(this->departure.get_start_place());
        t->set_clearance_speed(160);
		this->aircrafts.push_back(t);
        this->holdings.pop();
    }
}

void Game::update(double elapsed) {
	this->duration += elapsed;
    this->check_collision();
    this->handle_holdings();

    for (std::list <Aircraft*> :: iterator it = this->aircrafts.begin(); it != this->aircrafts.end(); ++it) {
        (*it)->set_separation_error(false);
        (*it)->update(elapsed);
		
		if ((*it)->get_speed() < 20) {
			it = this->aircrafts.erase(it);
			++this->handled_planes;
		}
    }

    for (std::list <Aircraft*> :: iterator it = errors.begin(); it != errors.end(); ++it) {
        (*it)->set_separation_error(true);
    }

    if (this->duration > this->new_plane) {
        create_plane();
        double time_for_next_plane = Tools::rnd(this->settings.new_plane_lower * 1000, this->settings.new_plane_upper * 1000);
		this->new_plane += time_for_next_plane;
    }
}

std::vector <Airfield>& Game::get_airfields() {
    return this->airfields;
}

double Game::get_duration() {
    return this->duration;
}

void Game::select_aircraft(std::string callsign) {
    this->selected = NULL;

    std::list <Aircraft*> :: iterator plane;

    for (plane = this->aircrafts.begin(); plane != this->aircrafts.end(); ++plane) {
        if (callsign == (*plane)->get_name()) {
            this->selected = (*plane);
			return;
        }
    }
}

void Game::create_plane() {
	std::clog << "Game::create_plane()" << std::endl;

	Inpoint t_inpoint = this->inpoints[Tools::rnd(0, (int)this->inpoints.size()-1)];
	Outpoint t_outpoint = this->outpoints[Tools::rnd(0, (int)this->outpoints.size()-1)];
	double heading = Tools::deg2rad(227);
	
	Coordinate test(60.46015, 25.2663);
	
    int type = Tools::rnd(1, 100);
	type = 49;

    std::string t_callsign = this->airlines(Tools::rnd(0, this->airlines.size()), "ICAO") + Tools::tostr(Tools::rnd(1, 999));
	
	Aircraft* plane;
	
	if (type >= 50) {
		plane = new Aircraft(t_callsign, 0.0, heading, this->active_field->get_altitude(), this->departure.get_start_place(), type, this->settings, this->landing);
		this->holdings.push(plane);
	} else {
		plane = new Aircraft(t_callsign, 200, heading, 2500, test, type, this->settings, this->landing);
		this->aircrafts.push_back(plane);
	}
}

void Game::load_airfield(std::string icao) {
	std::clog << "Game::load_airfield(" << icao << ")" << std::endl;
    Queryresult airport = Database::get_result("SELECT ROWID AS airfield_id, ICAO, latitude, longitude, altitude FROM airfields WHERE ICAO = '" + icao + "'");

    std::map <std::string, std::string> variables;
	
    variables["airfield_id"] = airport(0, "airfield_id");

    Coordinate place(Tools::tonumber<double>(airport(0, "latitude")), Tools::tonumber<double>(airport(0, "longitude")));

    this->active_field = new Airfield(airport(0, "ICAO"), place);
	this->settings.airfield_altitude = Tools::toint(airport(0, "altitude"));
	
	try {
		std::string query = Database::bind_param("SELECT name, latitude, longitude, altitude, heading, type FROM navpoints WHERE ? = ?", variables);
		
		Queryresult q_navpoints = Database::get_result(query);
		
		for (unsigned int i = 0; i < q_navpoints.size(); ++i) {
			double t_lat = Tools::tonumber<double>(q_navpoints(i, "latitude"));
			double t_lon = Tools::tonumber<double>(q_navpoints(i, "longitude"));

			Coordinate t_place(t_lat, t_lon);
			std::string t_name  = q_navpoints(i, "name");
			int type = Tools::toint(q_navpoints(i, "type"));

			if (type == 2) {
				double t_altitude   = Tools::tonumber<double>(q_navpoints(i, "altitude"));
				double t_heading    = Tools::tonumber<double>(q_navpoints(i, "heading"));

				this->inpoints.push_back(Inpoint(t_name, t_place, 250, t_altitude, t_heading));
			} else {
				this->outpoints.push_back((Outpoint(t_name, t_place)));
			}

			this->navpoints.insert(this->navpoints.end(), this->outpoints.begin(), this->outpoints.end());
			this->navpoints.insert(this->navpoints.end(), this->inpoints.begin(), this->inpoints.end());
		}
	} catch ( ... ) {
		throw;
	}
	
	try {
		std::string query = Database::bind_param("SELECT name, start_latitude, start_longitude, end_latitude, end_longitude FROM runways WHERE ? = ?", variables);
		
		Queryresult q_runways = Database::get_result(query);

		for (unsigned int i = 0; i < q_runways.size(); ++i) {
			double s_lat = Tools::tonumber<double>(q_runways(i, "start_latitude"));
			double s_lon = Tools::tonumber<double>(q_runways(i, "start_longitude"));
			double e_lat = Tools::tonumber<double>(q_runways(i, "end_latitude"));
			double e_lon = Tools::tonumber<double>(q_runways(i, "end_longitude"));
		
			Coordinate start_p(s_lat, s_lon);
			Coordinate end_p(e_lat, e_lon);

			Runway rwy(q_runways(i, "name"), start_p, end_p);
			
			this->active_field->add_runway(rwy);
		}
	} catch ( ... ) {
		throw;
	}
	
    this->centerpoint = this->active_field->get_place();
}

Aircraft* Game::get_selected() {
    return this->selected;
}

std::string Game::get_metar() {
	return "EFHK ...";
}

void Game::build_clearance(std::string command) {
	std::vector <std::string> tmp = Tools::split(" ", command);
	
	if (this->selected != NULL) {
		int value = Tools::tonumber<int>(tmp.back());

		if (Tools::trim(tmp[0]) == "turn") {
			int turn = (Tools::trim(tmp[1]) == "right") ? 1 : -1;
			
			this->selected->set_clearance_heading(Tools::deg2rad(value), turn);
		} else if (Tools::trim(tmp[0]) == "climb") {
			if (this->selected->get_altitude() > value) {
				std::cerr << "Can't climb, because altitude " << this->selected->get_altitude() << " ft is higher than " << value << " ft" << std::endl;
			} else {
				this->selected->set_clearance_altitude(value);
			}
		} else if (Tools::trim(tmp[0]) == "descent") {
			if (this->selected->get_altitude() < value) {
				std::cerr << "Can't descent, because altitude " << this->selected->get_altitude() << " ft is lower than " << value << " ft" << std::endl;
			} else {
				this->selected->set_clearance_altitude(value);
			}
		} else if (Tools::trim(tmp[0]) == "speed") {
			this->selected->set_clearance_speed(value);
		} else if (Tools::trim(tmp[0]) == "approach") {
			this->selected->set_clearance_approach();
		}
	} else {
		std::cerr << "No selected plane" << std::endl;
	}
}

int Game::get_handled_planes() {
	return this->handled_planes;
}

int Game::get_planes_count() {
	return this->aircrafts.size();
}

int Game::get_separation_errors() {
	return this->separation_errors;
}

int Game::get_new_plane() {
	return this->new_plane;
}