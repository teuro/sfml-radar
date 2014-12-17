#include "game.hpp"

Game::Game(Coordinate& cp, Settings& s) : center_point(cp), settings(s) {
    this->duration = 0;
    this->separation_errors = 0;
    this->cl_alt = -1;
    this->cl_spd = -1;
    this->cl_hdg = -1;
    this->new_plane = 5000;
}

Game::~Game() { }

void Game::load() {
    std::clog << "Game::load()" << std::endl;

    this->document.LoadFile("layout.xml");

    this->load_airfield("EFPO");

    this->duration = 0;

    create_plane();
    create_plane();
    create_plane();

    this->build_xml();
}

void Game::set_centerpoint(Coordinate& cp) {
    this->center_point = cp;
}

Coordinate& Game::get_centerpoint() {
    return this->center_point;
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

    this->cl_spd = -1;
    this->cl_hdg = -1;
    this->cl_alt = -1;
}

Point Game::get_place(Point& center, Coordinate& target) {
    Point target_place = Tools::calculate(center, this->center_point, target, this->settings.zoom);

    return target_place;
}

void Game::create_plane() {
    Queryresult airlines = Database::get_result("SELECT ICAO FROM airlines");
    Navpoint t_navpoint = this->navpoints[Tools::rnd(0, (int)this->navpoints.size())];

    std::string t_callsign = airlines(Tools::rnd(0, airlines.size()), "ICAO") + Tools::tostr(Tools::rnd(1, 999));
    this->aircrafts.push_back(new Aircraft(t_callsign, 250, 160, 9000, t_navpoint.get_place()));
}

std::vector <Runway>& Game::get_runways() {
    return this->active_field->get_runways();
}

void Game::load_airfield(std::string icao) {
    Queryresult airport = Database::get_result("SELECT ROWID AS airfield_id, ICAO, latitude, longitude FROM airfields WHERE ICAO = '" + icao + "'");

    std::stack <std::string> v;
    v.push(airport(0, "airfield_id"));
    v.push("airfield_id");

    Coordinate place(Tools::tonumber<double>(airport(0, "latitude")), Tools::tonumber<double>(airport(0, "longitude")));

    this->active_field = new Airfield(airport(0, "ICAO"), place);

    std::string query = Database::bind_param("SELECT name, latitude, longitude FROM navpoints WHERE ? = ? OR airfield_id = 1000", v);

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
    //std::clog << "Game::build_xml()" << std::endl;
    TiXmlElement* root = document.RootElement();

    if (root->NoChildren()) {
        std::clog << root->Value() << " has no childrens" << std::endl;
    } else {
        root->Clear();
    }

    TiXmlElement* planelist = new TiXmlElement("planelist");

    for (this->plane = this->aircrafts.begin(); this->plane != this->aircrafts.end(); ++this->plane) {
        TiXmlElement* t_plane = new TiXmlElement("plane");

        TiXmlElement* t_place = new TiXmlElement("place");
        TiXmlElement* t_lati = new TiXmlElement("latitude");
        TiXmlElement* t_long = new TiXmlElement("longitude");

        TiXmlElement* t_call = new TiXmlElement("callsign");
        t_call->LinkEndChild(new TiXmlText((*this->plane)->get_name().c_str()));

        TiXmlElement* t_spd = new TiXmlElement("speed");
        t_spd->LinkEndChild(new TiXmlText(Tools::tostr((*plane)->get_speed()).c_str()));

        TiXmlElement* t_alt = new TiXmlElement("altitude");
        t_alt->LinkEndChild(new TiXmlText(Tools::tostr((*plane)->get_altitude()).c_str()));

        TiXmlElement* t_hdg = new TiXmlElement("heading");
        t_hdg->LinkEndChild(new TiXmlText(Tools::tostr((*plane)->get_heading()).c_str()));

        TiXmlText* s_lat = new TiXmlText(Tools::tostr((*plane)->get_place().get_latitude()).c_str());
        TiXmlText* s_lon = new TiXmlText(Tools::tostr((*plane)->get_place().get_longitude()).c_str());

        t_lati->LinkEndChild(s_lat);
        t_long->LinkEndChild(s_lon);

        t_place->LinkEndChild(t_lati);
        t_place->LinkEndChild(t_long);

        t_plane->LinkEndChild(t_call);
        t_plane->LinkEndChild(t_place);
        t_plane->LinkEndChild(t_spd);
        t_plane->LinkEndChild(t_alt);
        t_plane->LinkEndChild(t_hdg);

        planelist->LinkEndChild(t_plane);
    }

    root->LinkEndChild(planelist);
    document.SaveFile();
}
