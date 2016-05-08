#include "game.hpp"

Game::Game(Settings& s) : settings(s) {
    this->duration = 0;
    this->separation_errors = 0;
    this->new_plane = 5000;
}

Game::~Game() { }

void Game::load(std::string airfield, std::string dep, std::string lnd) {
    std::clog << "Game::load(" << airfield << ", " << dep << ", " << lnd << ")" << std::endl;

    this->load_airfield(airfield);

    this->duration = 0;
	this->departure = this->active_field->get_runway(dep);
	this->landing = this->active_field->get_runway(lnd);
	
	std::clog << "Game is loaded" << std::endl;
}

void Game::set_centerpoint(Coordinate& cp) {
    this->centerpoint = cp;
}

Coordinate& Game::get_centerpoint() {
    return this->centerpoint;
}

Airfield* Game::get_active_field() {
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
			//std::clog << (*plane)->get_name() << " " << (*plane)->get_altitude() << std::endl;
            if ((*plane)->get_altitude() < 1000) {
                return;
            }

            ++plane;
        }

        Aircraft* t = this->holdings.front();
        t->set_place(this->departure.get_start_place());
		
		//std::clog << "Departure heading is " << Tools::rad2deg(this->departure->get_heading()+3.14/2.0) << std::endl;
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
    }

    for (std::list <Aircraft*> :: iterator it = errors.begin(); it != errors.end(); ++it) {
        (*it)->set_separation_error(true);
    }

    if (this->duration > this->new_plane) {
        create_plane();
        double t = Tools::rnd(this->settings.new_plane_lower * 1000, this->settings.new_plane_upper * 1000);
        this->new_plane += t;
    }
}

std::vector <Airfield>& Game::get_airfields() {
    return this->airfields;
}

double Game::get_duration() {
    return this->duration;
}

void Game::select_aircraft(Point& mouse) {
    this->selected = NULL;

    std::list <Aircraft*> :: iterator plane;

    for (plane = this->aircrafts.begin(); plane != this->aircrafts.end(); ++plane) {
        Point tmp(this->settings.screen_width/2, this->settings.screen_height/2);
        Point aircraft = Tools::calculate(tmp, this->centerpoint, (*plane)->get_place(), this->settings.zoom);

        if (Tools::on_area(mouse, aircraft)) {
            this->selected = (*plane);
        }
    }
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
    Queryresult airlines = Database::get_result("SELECT ICAO FROM airlines");
	Inpoint t_inpoint = this->inpoints[Tools::rnd(0, (int)this->inpoints.size()-1)];
	Outpoint t_outpoint = this->outpoints[Tools::rnd(0, (int)this->outpoints.size()-1)];
	double heading = this->departure.get_heading();
	
    int type = Tools::rnd(1, 100);
	type = 49;

    std::string t_callsign = airlines(Tools::rnd(0, airlines.size()), "ICAO") + Tools::tostr(Tools::rnd(1, 999));
	
	Aircraft* plane;
	
	if (type >= 50) {
		plane = new Aircraft(t_callsign, 120.0, heading, this->active_field->get_altitude(), this->departure.get_start_place(), type, this->settings, this->landing);
		this->holdings.push(plane);
	} else {
		plane = new Aircraft(t_callsign, 250.0, heading, 10000, t_outpoint.get_place(), type, this->settings, this->landing);
		this->aircrafts.push_back(plane);
	}
}

void Game::load_airfield(std::string icao) {
    Queryresult airport = Database::get_result("SELECT ROWID AS airfield_id, ICAO, latitude, longitude FROM airfields WHERE ICAO = '" + icao + "'");

    std::map <std::string, std::string> variables;
	
    variables["airfield_id"] = airport(0, "airfield_id");

    Coordinate place(Tools::tonumber<double>(airport(0, "latitude")), Tools::tonumber<double>(airport(0, "longitude")));

    this->active_field = new Airfield(airport(0, "ICAO"), place);
	
	try {
		std::string query = Database::bind_param("SELECT name, latitude, longitude, altitude, heading, type FROM navpoints WHERE ? = ?", variables);
		
		Queryresult q_navpoints = Database::get_result(query);
		
		for (unsigned int i = 0; i < q_navpoints.size(); ++i) {
			double t_lat = Tools::tonumber<double>(q_navpoints(i, "latitude"));
			double t_lon = Tools::tonumber<double>(q_navpoints(i, "longitude"));

			Coordinate t_place(t_lat, t_lon);
			std::string t_name  = q_navpoints(i, "name");
			int type = Tools::tonumber<int>(q_navpoints(i, "type"));

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
	std::clog << "Game::load_airfield(" << icao << ") => " << this->centerpoint << std::endl; 
}

Aircraft* Game::get_selected() {
    return this->selected;
}

std::string Game::get_metar() {
	return "EFHK ...";
}

void Game::build_clearance(std::string command) {
	std::clog << "Game::build_clearance(" << command << ")" << std::endl;
	
	std::vector <std::string> tmp = Tools::split(" ", command);
	std::clog << "vector tmp has " << tmp.size() << " items" << std::endl;
	int turn = 0;
	int hdg;
	int spd;
	int alt;
	std::string rwy;
	
	if (tmp[0] == "turn") {
		turn = (tmp[1] == "right") ? 1 : -1;
		hdg = Tools::tonumber<int>(tmp.back());
		
		std::clog << "turn " << turn << " heading " << hdg << std::endl;
		this->selected->set_clearance_heading(Tools::deg2rad(hdg), turn);
	} else if (tmp[0] == "climb" || tmp[0] == "descent") {
		alt = Tools::toint(tmp.back());
		this->selected->set_clearance_altitude(alt);
	} else if (tmp[0] == "speed") {
		spd = Tools::toint(tmp.back());
		this->selected->set_clearance_speed(spd);
	} else if (tmp[0] == "approach") {
		rwy = tmp.back();
		this->selected->set_clearance_approach();
	}
}
