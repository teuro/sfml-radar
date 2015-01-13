#include "game_view.hpp"

Gameview::Gameview(Drawsurface& d, Settings& s) : View(d), settings(s) { }

Gameview::~Gameview() { }

void Gameview::load() {
    View::load();
    this->center_point.set_place(this->settings.screen_width / 2, this->settings.screen_height/2);
    TinyXPath::xpath_processor element(this->document.RootElement(), "//element");
    TinyXPath::xpath_processor plane(this->document.RootElement(), "//plane");
    TinyXPath::xpath_processor metar(this->document.RootElement(), "//metar");

    unsigned int elements_size = element.u_compute_xpath_node_set();
    unsigned int planes_size = plane.u_compute_xpath_node_set();
    unsigned int metar_size = metar.u_compute_xpath_node_set();

    for (unsigned int i = 0; i < elements_size; ++i) {
        TiXmlElement* t = element.XNp_get_xpath_node(i)->ToElement();

        std::string name = element.XNp_get_xpath_node(i)->FirstChild()->FirstChild()->Value();
        std::string id = t->Attribute("id");
        std::string cl = t->Attribute("class");
        Layout_element tmp(name, id, cl);
        this->layout_elements[name] = tmp;
    }

    std::string content;

    for (unsigned int i = 0; i < planes_size; ++i) {
        content += plane.XNp_get_xpath_node(i)->FirstChild()->Value();
        content += ";";
    }

    this->layout_elements["Planelist"].set_content(content);

    if (metar_size) {
        std::string t_m = metar.XNp_get_xpath_node(0)->FirstChild()->Value();
        t_m += ";";
        this->layout_elements["Metar"].set_content(t_m);
    } else {
        throw std::runtime_error("Metar not found layout.xml");
    }

    if (this->command.length()) {
        this->layout_elements["Input"].set_content(this->command);
    }

    std::map <std::string, Layout_element> :: iterator le;

    for (le = this->layout_elements.begin(); le != this->layout_elements.end(); ++le) {
        this->style(le->second);
    }
}

void Gameview::draw() {
    //std::clog << "Gameview::draw()" << std::endl;
    this->load();

    std::map <std::string, Layout_element> :: iterator le;

    for (le = this->layout_elements.begin(); le != this->layout_elements.end(); ++le) {
        this->draw_element(le->second);
    }
}

void Gameview::draw_plane(Aircraft*& plane, Point& center_point, std::string color) {
    Point aircraft_place = Tools::calculate(this->center_point, this->settings.centerpoint, plane->get_place(), this->settings.zoom);

    #ifdef DEBUG
    std::clog << plane->get_name() << " " << aircraft_place.get_x() << ", " << aircraft_place.get_y() << std::endl;
    #endif
    double minute_px        = Tools::distancePX(plane->get_speed() * (1.0 / 60.0), this->settings.zoom, this->settings.screen_width);
    double separation_ring  = Tools::distancePX(settings.separation_horizontal, this->settings.zoom, this->settings.screen_width);

    Point end_point = Tools::calculate(aircraft_place, plane->get_heading(), minute_px, false, true);
    drawer.lineColor(aircraft_place, end_point, color);
    drawer.circleColor(aircraft_place, separation_ring, color);
    drawer.rectangleColor(aircraft_place, 10, color);
    aircraft_place.change_x(10);
    drawer.draw_text(plane->get_name(), aircraft_place, color);
    aircraft_place.change_y(20);
    drawer.draw_text(Tools::tostr(plane->get_speed()), aircraft_place, color);
    aircraft_place.change_y(20);
    drawer.draw_text(Tools::tostr(plane->get_heading()), aircraft_place, color);
    aircraft_place.change_y(20);
    drawer.draw_text(Tools::tostr(plane->get_altitude()), aircraft_place, color);
}

void Gameview::draw_navpoints(std::vector <Navpoint>& navpoints) {
    for (unsigned int i = 0; i < navpoints.size(); ++i) {
        Point place_screen = Tools::calculate(this->center_point, this->settings.centerpoint, navpoints[i].get_place(), this->settings.zoom);

        this->drawer.trigonColor(place_screen, 5, "green");
        this->drawer.draw_text(navpoints[i].get_name(), place_screen, "green");
    }
}

void Gameview::draw_planes(std::list <Aircraft*> planes, Aircraft* selected) {
    std::list <Aircraft*> :: iterator plane = planes.begin();
    std::string color = "blue";

    while (plane != planes.end()) {
        color = "blue";

        if (selected != NULL && (*plane) == selected) {
            color = "white";
        }

        this->draw_plane((*plane), this->center_point, color);

        ++plane;
    }
}

void Gameview::draw_airfield(Airfield* airfield) {
    std::vector <Runway> runways = airfield->get_runways();

    for (unsigned int i = 0; i < runways.size(); ++i) {
        Point rwys = Tools::calculate(this->center_point, this->settings.centerpoint, runways[i].get_start_place(), this->settings.zoom);
        Point rwye = Tools::calculate(this->center_point, this->settings.centerpoint, runways[i].get_end_place(), this->settings.zoom);

        this->drawer.lineColor(rwys, rwye, "white");
    }
}
