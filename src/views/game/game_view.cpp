#include "game_view.hpp"

Gameview::Gameview(Drawsurface& d, Settings& s) : View(d), settings(s) { }

Gameview::~Gameview() { }

void Gameview::load() {
    this->styles = parse("style.css");
    this->center_point.set_place(this->settings.screen_width / 2, this->settings.screen_height/2);
    this->document.LoadFile("layout.xml");
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

    if (this->callsign.length()) {
        this->layout_elements["Input"].set_content(this->callsign);
    }

    if (this->command.length()) {
        this->layout_elements["Input"].set_content(this->command);
    }

    if (this->reply.length()) {
        if (this->layout_elements["Reply"].get_content().size() == 0) {
            this->layout_elements["Reply"].set_content(this->reply);
        }
    }

    std::map <std::string, Layout_element> :: iterator le;

    for (le = this->layout_elements.begin(); le != this->layout_elements.end(); ++le) {
        this->style(le->second);
    }
}

void Gameview::clear_screen() {
    drawer.clear_screen();
}

void Gameview::draw() {
    //std::clog << "Gameview::draw()" << std::endl;
    this->load();

    std::map <std::string, Layout_element> :: iterator le;

    for (le = this->layout_elements.begin(); le != this->layout_elements.end(); ++le) {
        this->draw_element(le->second);
    }
}

void Gameview::render() {
    drawer.flip();
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

void Gameview::draw_element(Layout_element& layout_element) {
    if (layout_element.b_color_setted) {
        drawer.rectangleColor(layout_element.get_top_left(), layout_element.get_bottom_right(), layout_element.b_red, layout_element.b_green, layout_element.b_blue, true);
    } else {
        drawer.rectangleColor(layout_element.get_top_left(), layout_element.get_bottom_right(), "black", true);
    }
    Point t = layout_element.get_top_left();

    t.change_y(-8);
    if (layout_element.t_color_setted) {
        drawer.draw_text(layout_element.get_name(), t, layout_element.t_red, layout_element.t_green, layout_element.t_blue, 10);
    } else {
        drawer.draw_text(layout_element.get_name(), t, "blue", 10);
    }
    t.change_y(8);

    std::vector <std::string> content = layout_element.get_content();

    for (unsigned int i = 0; i < content.size(); ++i) {
        if (layout_element.t_color_setted) {
            drawer.draw_text(content[i], t, layout_element.t_red, layout_element.t_green, layout_element.t_blue);
        } else {
            drawer.draw_text(content[i], t, "blue");
        }
        t.change_y(drawer.get_fontsize());
    }
}

void Gameview::draw_navpoints(std::vector <Navpoint>& navpoints) {
    for (unsigned int i = 0; i < navpoints.size(); ++i) {
        Point place_screen = Tools::calculate(this->center_point, this->settings.centerpoint, navpoints[i].get_place(), this->settings.zoom);

        this->drawer.trigonColor(place_screen, 5, "green");
        this->drawer.draw_text(navpoints[i].get_name(), place_screen, "green");
    }
}

bool compare_length(std::string const& lhs, std::string const& rhs) {
    return lhs.size() < rhs.size();
}

void Gameview::style(Layout_element& le) {
    std::list <Style> :: iterator t_style = this->styles.begin();

    while (t_style != this->styles.end()) {
        if (le == t_style->get_id()) {
            Point p(t_style->get_left(), t_style->get_top());
            le.set_place(p);

            int t_color = t_style->get_t_color();
            int b_color = t_style->get_b_color();

            if (t_color >= 0) {
                int blue = t_color % 256;
                t_color /= 256;
                int red = t_color / 256;
                int green = t_color % 256;

                le.t_red = red;
                le.t_green = green;
                le.t_blue = blue;

                le.t_color_setted = true;
            }

            if (b_color >= 0) {
                int blue = b_color % 256;
                b_color /= 256;
                int red = b_color / 256;
                int green = b_color % 256;

                le.b_red = red;
                le.b_green = green;
                le.b_blue = blue;

                le.b_color_setted = true;
            }

            if (le.get_content().size() * drawer.get_fontsize() > t_style->get_height()) {
                t_style->set_height((le.get_content().size()-1) * drawer.get_fontsize() + 30);
            }

            std::vector <std::string> lines = le.get_content();
            std::string longest_line;
            std::vector <std::string> :: iterator longest;

            if (lines.size() == 0) {
                longest_line = "";
            } else if (lines.size() < 2) {
                longest_line = lines[0];
            } else {
                longest = std::max_element(lines.begin(), lines.end(), compare_length);
                longest_line = *longest;
            }

            if (longest_line.length() * (drawer.get_fontsize()/3) > t_style->get_width()) {
                t_style->set_width(longest_line.length() * (drawer.get_fontsize()/3)+40);
            }

            le.set_size(t_style->get_width(), t_style->get_height());
        }

        ++t_style;
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

void Gameview::set_command(std::string command) {
    this->command = command;
}

void Gameview::update_command(std::string command) {
    this->command = command;
}

std::string Gameview::get_command() {
    return (this->callsign + this->command);
}

void Gameview::set_reply(std::string reply) {
    this->reply = reply;
}
