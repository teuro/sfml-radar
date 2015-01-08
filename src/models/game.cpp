#include "game.hpp"

Game::Game(Coordinate& cp, Settings& s) : center_point(cp), settings(s) {
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

void Game::update(double elapsed) {
    this->duration += elapsed;

    std::list <Aircraft*> :: iterator plane_a;
    std::list <Aircraft*> :: iterator plane_b;

    std::list <Aircraft*> errors;

    for (plane_a = this->aircrafts.begin(); plane_a != this->aircrafts.end(); ++plane_a) {
        for (plane_b = this->aircrafts.begin(); plane_b != this->aircrafts.end(); ++plane_b) {
            if (*plane_a == *plane_b) {
                continue;
            } else {
                if (std::abs((*plane_a)->get_altitude() - (*plane_b)->get_altitude()) < this->settings.separation_vertical) {
                    if (Tools::distanceNM((*plane_a)->get_place(), (*plane_b)->get_place()) < this->settings.separation_horizontal) {
                        errors.push_back((*plane_a));
                        errors.push_back((*plane_b));
                        if (!(*plane_a)->get_separation_error() || !(*plane_b)->get_separation_error()) {
                            std::clog << "Separation error occured between " << (*plane_a)->get_name() << " and " << (*plane_b)->get_name() << std::endl;
                            ++this->separation_errors;
                        }
                    }
                }
            }
        }

        (*plane_a)->update(elapsed);

    }

    for (std::list <Aircraft*> :: iterator it = this->aircrafts.begin(); it != this->aircrafts.end(); ++it) {
        (*it)->set_separation_error(false);
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
        Point aircraft = this->get_place(tmp, (*plane)->get_place());

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

Point Game::get_place(Point& center, Coordinate& target) {
    //std::clog << "Game::get_place(" << center.get_x() << ", " << center.get_y() << ", " << target.get_latitude() << ", " << target.get_longitude() << ")" << std::endl;
    Point target_place = Tools::calculate(center, this->center_point, target, this->settings.zoom);

    return target_place;
}

void Game::create_plane() {
    Queryresult airlines = Database::get_result("SELECT ICAO FROM airlines");
    Navpoint t_navpoint = this->navpoints[Tools::rnd(0, (int)this->navpoints.size())];

    int type = Tools::rnd(1, 100);

    std::string t_callsign = airlines(Tools::rnd(0, airlines.size()), "ICAO") + Tools::tostr(Tools::rnd(1, 999));

    Aircraft* tmp;

    if (type > 50) {
        /** Departure **/
        tmp = new Aircraft(t_callsign, 0, 0, this->active_field->get_altitude(), t_navpoint.get_place(), type);
        this->holdings.push(tmp);
        std::clog << "Holding planes " << this->holdings.size() << std::endl;
    } else {
        /** Arrival **/
        tmp = new Aircraft(t_callsign, 250, 160, 9000, t_navpoint.get_place(), type);
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

    std::string query = Database::bind_param("SELECT name, latitude, longitude FROM navpoints WHERE ? = ?", v);

    Queryresult q_navpoints = Database::get_result(query);
    for (unsigned int i = 0; i < q_navpoints.size(); ++i) {

        double t_lat = Tools::tonumber<double>(q_navpoints(i, "latitude"));
        double t_lon = Tools::tonumber<double>(q_navpoints(i, "longitude"));

        Coordinate tplace(t_lat, t_lon);
        this->navpoints.push_back(Navpoint(q_navpoints(i, "name"), tplace));
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
    TiXmlElement* element3 = new TiXmlElement("metar");
    TiXmlElement* element4 = new TiXmlElement("input");

    TiXmlElement* name1 = new TiXmlElement("name");
    TiXmlElement* name2 = new TiXmlElement("name");
    TiXmlElement* name3 = new TiXmlElement("name");
    TiXmlElement* name4 = new TiXmlElement("name");

    TiXmlText* t1 = new TiXmlText("Planelist");
    TiXmlText* t2 = new TiXmlText("Clearance box");
    TiXmlText* t3 = new TiXmlText("Metar");
    TiXmlText* t4 = new TiXmlText("Input");

    name1->LinkEndChild(t1);
    name2->LinkEndChild(t2);
    name3->LinkEndChild(t3);
    name4->LinkEndChild(t4);

    element1->SetAttribute("id", "planelist");
    element1->SetAttribute("class", "data");

    element2->SetAttribute("id", "clearance-box");
    element2->SetAttribute("class", "data");

    element1->LinkEndChild(name1);
    element2->LinkEndChild(name2);

    elements->LinkEndChild(element2);

    std::list <Aircraft*> :: iterator plane;

    for (plane = this->aircrafts.begin(); plane != this->aircrafts.end(); ++plane) {
        TiXmlElement* t_plane = new TiXmlElement("plane");

        t_plane->LinkEndChild(new TiXmlText((*plane)->get_name().c_str()));
        element1->LinkEndChild(t_plane);
    }

    elements->LinkEndChild(element1);

    TiXmlElement* e_metar = new TiXmlElement("element");
    e_metar->SetAttribute("id", "metar");
    e_metar->SetAttribute("class", "data");
    e_metar->LinkEndChild(name3);
    TiXmlText* t_metar = new TiXmlText("EFHK 301250Z 27006KT 2000 +RA BKN012 03/02 Q0998");
    element3->LinkEndChild(t_metar);
    e_metar->LinkEndChild(element3);

    TiXmlElement* e_input = new TiXmlElement("element");
    e_input->SetAttribute("id", "input");
    e_input->SetAttribute("class", "data");
    e_input->LinkEndChild(name4);
    e_input->LinkEndChild(element4);

    elements->LinkEndChild(e_metar);
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
        }
    } else {
        throw std::runtime_error("Unknown command please try again");
    }
}
