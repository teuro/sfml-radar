#include "game_view.hpp"

Gameview::Gameview(Game& g, Drawsurface& d, Settings& s) : View(d), game(g), settings(s) {
    this->load();
}

Gameview::~Gameview() { }

void Gameview::load() {
    //std::clog << "Gameview::load()" << std::endl;
    this->center_point.set_place(this->settings.screen_width / 2, this->settings.screen_height/2);
    this->document.LoadFile("layout.xml");
    TinyXPath::xpath_processor test(this->document.RootElement(), "//planelist/plane");
/*
    for (unsigned int i = 0; i < test.u_compute_xpath_node_set(); ++i) {
        TiXmlNode* plane = test.XNp_get_xpath_node(i);

        std::string callsing    = plane->FirstChildElement()->GetText();
        double latitude         = Tools::tonumber<double>(plane->FirstChildElement()->NextSiblingElement()->FirstChildElement()->GetText());
        double longitude        = Tools::tonumber<double>(plane->FirstChildElement()->NextSiblingElement()->FirstChildElement()->NextSiblingElement()->GetText());
        double speed            = Tools::tonumber<double>(plane->FirstChildElement()->NextSiblingElement()->NextSiblingElement()->GetText());
        double altitude         = Tools::tonumber<double>(plane->FirstChildElement()->NextSiblingElement()->NextSiblingElement()->NextSiblingElement()->GetText());
        double heading          = Tools::tonumber<double>(plane->FirstChildElement()->NextSiblingElement()->NextSiblingElement()->NextSiblingElement()->NextSiblingElement()->GetText());
    }
    */
}

void Gameview::draw() {
    this->load();

    drawer.clear_screen();

    for (unsigned int i = 0; i < this->layout_elements.size(); ++i) {
        this->draw_element(this->layout_elements[i]);
    }

	drawer.flip();
}

void Gameview::draw_plane(Aircraft*& plane, Point& center_point, std::string color) {
    this->draw_plane(plane->get_name(), plane->get_place().get_latitude(), plane->get_place().get_longitude(), plane->get_speed(), plane->get_altitude(), plane->get_heading(), this->center_point, color);
}

void Gameview::draw_plane(std::string callsign, double latitude, double longitude, double speed, double altitude, double heading, Point& center_point, std::string color) {
    Coordinate place(latitude, longitude);
    Point aircraft_place = this->game.get_place(center_point, place);

    double minute_px        = Tools::distancePX(speed * (1.0 / 60.0), this->settings.zoom, this->settings.screen_width);
    double separation_ring  = Tools::distancePX(settings.separation_horizontal, this->settings.zoom, this->settings.screen_width);

    Point end_point = Tools::calculate(aircraft_place, heading, minute_px, false, true);
    drawer.lineColor(aircraft_place, end_point, color);
    drawer.circleColor(aircraft_place, separation_ring, color);
    drawer.rectangleColor(aircraft_place, 10, color);
    aircraft_place.change_x(10);
    drawer.draw_text(callsign, aircraft_place, color);
    aircraft_place.change_y(20);
    drawer.draw_text(Tools::tostr(speed), aircraft_place, color);
    aircraft_place.change_y(20);
    drawer.draw_text(Tools::tostr(heading), aircraft_place, color);
    aircraft_place.change_y(20);
    drawer.draw_text(Tools::tostr(altitude), aircraft_place, color);
}

void Gameview::draw_clearance(Aircraft*& plane, Point& place, std::string color) {
    int changex = 80;
    if (this->game.get_cl_spd() < 0) {
        this->game.set_cl_values(plane->get_clearance_speed(), plane->get_clearance_altitude(), plane->get_clearance_heading());
    }

    drawer.draw_text("Title", place, color);
    place.change_x(changex);
    drawer.draw_text("Present", place, color);
    place.change_x(changex);
    drawer.draw_text("Clearance", place, color);
    place.change_x(-changex*2);
    place.change_y(drawer.get_fontsize());

    drawer.draw_text("Speed", place, color);
    place.change_x(changex);
    drawer.draw_text(Tools::tostr(plane->get_speed()), place, color);
    place.change_x(changex);
    drawer.draw_text(Tools::tostr(this->game.get_cl_spd()), place, color);

    place.change_x(-changex*2);
    place.change_y(drawer.get_fontsize());

    drawer.draw_text("Heading", place, color);
    place.change_x(changex);
    drawer.draw_text(Tools::tostr(plane->get_heading()), place, color);
    place.change_x(changex);
    drawer.draw_text(Tools::tostr(this->game.get_cl_hdg()), place, color);

    place.change_x(-changex*2);
    place.change_y(drawer.get_fontsize());

    drawer.draw_text("Altidude", place, color);
    place.change_x(changex);
    drawer.draw_text(Tools::tostr(plane->get_altitude()), place, color);
    place.change_x(changex);
    drawer.draw_text(Tools::tostr(this->game.get_cl_alt()), place, color);
}

Layout_element& Gameview::get_element(std::string name) {
    for (unsigned int i = 0; i < layout_elements.size(); ++i) {
        if (layout_elements[i].get_name() == name) {
            return layout_elements[i];
        }
    }

    throw std::logic_error("Requested element " + name + " is not part of layout_elements in Gameviev");
}

void Gameview::draw_element(Layout_element& layout_element) {
    drawer.rectangleColor(layout_element.get_top_left(), layout_element.get_bottom_right(), "brown", true);
    Point t = layout_element.get_top_left();
    t.change_y(-8);
    drawer.draw_text(layout_element.get_name(), t, "blue", 10);
}
