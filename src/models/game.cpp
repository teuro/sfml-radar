#include "game.hpp"

Game::Game(Coordinate& cp, Settings& s, Atis& a) : center_point(cp), settings(s), atis(a) {
    this->duration = 0;
    this->separation_errors = 0;
    this->new_plane = 5000;
}

Game::~Game() { }

void Game::load() {
    std::clog << "Game::load()" << std::endl;

    this->load_airfield("EFHK");

    this->duration = 0;

    create_plane();
    create_plane();
    create_plane();

    this->metar.update();
}

void Game::set_centerpoint(Coordinate& cp) {
    this->center_point = cp;
}

Coordinate& Game::get_centerpoint() {
    return this->center_point;
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

void Game::check_collision(std::list <Aircraft*>& planes) {
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
        t->set_place(this->departure->get_start_place());
        Clearance cl(160, this->active_field->get_altitude(), this->departure->get_heading(), 1);
        this->holdings.pop();
        this->aircrafts.push_back(t);
    }
}

void Game::update(double elapsed) {
    this->duration += elapsed;
    this->check_collision(this->aircrafts);
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

    this->build_xml();
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
        Point aircraft = Tools::calculate(tmp, this->center_point, (*plane)->get_place(), this->settings.zoom);

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
        }
    }
}

void Game::create_plane() {
    Queryresult airlines = Database::get_result("SELECT ICAO FROM airlines");
    Inpoint t_navpoint = this->inpoints[Tools::rnd(0, (int)this->inpoints.size())];

    int type = Tools::rnd(1, 100);

    if (!is_free(t_navpoint)) {
        type = 51;
    }

    std::string t_callsign = airlines(Tools::rnd(0, airlines.size()), "ICAO") + Tools::tostr(Tools::rnd(1, 999));

    Aircraft* tmp;

    if (type > 50) {
        /** Departure **/
        tmp = new Aircraft(t_callsign, 0, 0, this->active_field->get_altitude(), t_navpoint.get_place(), type, settings);
        this->holdings.push(tmp);
        std::clog << "Holding planes " << this->holdings.size() << std::endl;
    } else {
        /** Arrival **/
        tmp = new Aircraft(t_callsign, 200, 40, 3000, t_navpoint.get_place(), type, settings);
        this->aircrafts.push_back(tmp);
    }
}

void Game::load_airfield(std::string icao) {
    Queryresult airport = Database::get_result("SELECT ROWID AS airfield_id, ICAO, latitude, longitude FROM airfields WHERE ICAO = '" + icao + "'");

    std::stack <std::string> v;
    v.push(airport(0, "airfield_id"));
    v.push("airfield_id");

    Coordinate place(Tools::tonumber<double>(airport(0, "latitude")), Tools::tonumber<double>(airport(0, "longitude")));

    this->active_field = new Airfield(airport(0, "ICAO"), place);

    std::string query = Database::bind_param("SELECT name, latitude, longitude, altitude, heading, type FROM navpoints WHERE ? = ?", v);

    Queryresult q_navpoints = Database::get_result(query);
    for (unsigned int i = 0; i < q_navpoints.size(); ++i) {

        double t_lat = Tools::tonumber<double>(q_navpoints(i, "latitude"));
        double t_lon = Tools::tonumber<double>(q_navpoints(i, "longitude"));

        Coordinate t_place(t_lat, t_lon);
        std::string t_name  = q_navpoints(i, "name");
        int type = Tools::tonumber<int>(q_navpoints(i, "type"));

        if (type == 1) {
            double t_altitude   = Tools::tonumber<double>(q_navpoints(i, "altitude"));
            double t_heading    = Tools::tonumber<double>(q_navpoints(i, "heading"));

            this->inpoints.push_back(Inpoint(t_name, t_place, 250, t_altitude, t_heading));
        } else {
            this->outpoints.push_back((Outpoint(t_name, t_place)));
        }

        this->navpoints.insert(this->navpoints.end(), this->outpoints.begin(), this->outpoints.end());
        this->navpoints.insert(this->navpoints.end(), this->inpoints.begin(), this->inpoints.end());
    }

    query = Database::bind_param("SELECT name, start_latitude, start_longitude, end_latitude, end_longitude FROM runways WHERE ? = ?", v);
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

    this->center_point = this->active_field->get_place();
}

void Game::build_xml() {
    this->document.LoadFile("layout.xml");
    TiXmlElement* root = document.RootElement();

    document.RootElement()->Clear();

    TiXmlElement* elements = new TiXmlElement("elements");
    TiXmlElement* element1 = new TiXmlElement("element");
    TiXmlElement* element2 = new TiXmlElement("element");
    TiXmlElement* element4 = new TiXmlElement("input");

    element1->SetAttribute("id", "planelist");
    element1->SetAttribute("class", "data");
    element1->SetAttribute("name", "Planelist");

    element2->SetAttribute("id", "atis-box");
    element2->SetAttribute("class", "data");
    element2->SetAttribute("name", "Atis Box");

    elements->LinkEndChild(element2);

    std::list <Aircraft*> :: iterator plane;

    for (plane = this->aircrafts.begin(); plane != this->aircrafts.end(); ++plane) {
        TiXmlElement* t_plane = new TiXmlElement("plane");

        t_plane->LinkEndChild(new TiXmlText((*plane)->get_name().c_str()));
        element1->LinkEndChild(t_plane);
    }

    elements->LinkEndChild(element1);

    TiXmlElement* e_input = new TiXmlElement("element");
    e_input->SetAttribute("id", "input");
    e_input->SetAttribute("class", "data");
    e_input->SetAttribute("name", "Input");
    TiXmlText* t_input = new TiXmlText(this->command.c_str());
    element4->LinkEndChild(t_input);
    e_input->LinkEndChild(element4);

    elements->LinkEndChild(e_input);
    root->LinkEndChild(elements);
    document.SaveFile();
}

int Game::get_separation_errors() {
    return this->separation_errors;
}

Aircraft* Game::get_selected() {
    return this->selected;
}

double Game::get_next_plane() {
    return this->new_plane;
}

void Game::set_clearance(std::string callsign, std::vector <std::string> command) {
    this->select_aircraft(callsign);

    if (this->selected != NULL) {
        int turn = -1;
        int act_spd = this->selected->get_clearance_speed();
        int act_hdg = this->selected->get_clearance_heading();
        int act_alt = this->selected->get_clearance_altitude();

        if (this->selected->get_turn() == -1 || this->selected->get_turn() == 1) {
            turn = this->selected->get_turn();
        }

        if (command[0] == "turn") {
            int cl_gdg;

            if (command.size() == 3) {
                turn = (command[2] == "left") ? -1 : 1;
            }

            cl_gdg = Tools::tonumber<int>(command.back());

            Clearance t(act_spd, act_alt, cl_gdg, turn);
            this->selected->set_clearance(t);
        } else if (command[0] == "speed") {
            int cl_spd = Tools::tonumber<int>(command[1]);
            Clearance t(cl_spd, act_alt, act_hdg, turn);
            this->selected->set_clearance(t);
        } else if (command[0] == "climb" || command[0] == "descent") {
            int cl_alt = Tools::tonumber<int>(command[1]);
            Clearance t(act_spd, cl_alt, act_hdg, turn);
            this->selected->set_clearance(t);
        } else if (command[0] == "approach") {
            std::clog << this->selected->get_name() << " ";
            std::clog << this->selected->get_altitude() << " ";
            std::clog << this->selected->get_speed() << " ";
            std::clog << this->selected->get_heading() << " ";
            std::clog << (Tools::rad2deg(this->landing->get_heading()) + 30) << " ";
            std::clog << (Tools::rad2deg(this->landing->get_heading()) - 30) << " ";

            if (this->selected->get_altitude() <= 3000 &&
                this->selected->get_speed() <= 200 &&
                this->selected->get_heading() >= (Tools::rad2deg(this->landing->get_heading()) - 30) &&
                Tools::rad2deg(this->selected->get_heading() <= (this->selected->get_heading()) + 30)) {
                Clearance cl(this->landing);
                this->selected->set_clearance(cl);
            }
        } else if (command[0] == "direct") {
            Navpoint* t = &(*std::find(this->navpoints.begin(), this->navpoints.end(), command[1]));
            this->selected->set_target(t);
        }
    } else {

    }
}

Metar& Game::get_metar() {
    return this->metar;
}

void Game::set_active_runways(Runway* dep, Runway* lnd) {
    this->departure = dep;
    this->landing = lnd;
}

void Game::set_command(std::string command) {
    this->command = command;
}

std::string Game::get_command() {
    return this->command;
}

std::string Game::get_departure() {
    return this->atis.get_departure_runway();
}

std::string Game::get_landing() {
    return this->atis.get_landing_runway();
}

std::string Game::get_transition_altitude() {
    return Tools::tostr(this->atis.get_transition_altitude());
}

std::string Game::get_transition_level() {
    return Tools::tostr(this->atis.get_transition_level());
}
