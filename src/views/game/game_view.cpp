#include "game_view.hpp"

Gameview::Gameview(Drawsurface& d, Settings& s) : View(d, s) { }

Gameview::~Gameview() { }

void Gameview::load() {
	std::clog << "Gameview::load()" << std::endl;
	View::load();
}

void Gameview::draw() {
	this->center_point.set_place(this->settings.screen_width / 2, this->settings.screen_height/2);
	this->View::draw();
}

void Gameview::draw_plane(Aircraft*& plane, std::string color) {
	Point aircraft_place = Tools::calculate(this->center_point, this->centerpoint, plane->get_place(), this->settings.zoom);

    double separation_ring  = Tools::distancePX(settings.separation_horizontal / 2.0, this->settings.zoom, this->settings.screen_width);
	
	Coordinate end_point_c = Tools::calculate(plane->get_place(), plane->get_heading(), plane->get_speed() * (1.0 / 60.0));
	Point end_point_p = Tools::calculate(this->center_point, this->centerpoint, end_point_c, this->settings.zoom);
	
    drawer.lineColor(aircraft_place, end_point_p, color);
    drawer.circleColor(aircraft_place, separation_ring, color);
    drawer.rectangleColor(aircraft_place, 10, color);
    aircraft_place.change_x(10);
    drawer.draw_text(plane->get_name(), aircraft_place, color);
    aircraft_place.change_y(20);
    drawer.draw_text(Tools::tostr(plane->get_speed()), aircraft_place, color);
    aircraft_place.change_y(20);
    drawer.draw_text(Tools::tostr(Tools::rad2deg(plane->get_heading())), aircraft_place, color);
    aircraft_place.change_y(20);
    drawer.draw_text(Tools::tostr(plane->get_altitude()), aircraft_place, color);
}

void Gameview::draw_navpoints(std::vector <Navpoint>& navpoints) {
    for (unsigned int i = 0; i < navpoints.size(); ++i) {
        Point place_screen = Tools::calculate(this->center_point, this->centerpoint, navpoints[i].get_place(), this->settings.zoom);

        this->drawer.trigonColor(place_screen, 5);
        this->drawer.draw_text(navpoints[i].get_name(), place_screen, "green");
    }
}

void Gameview::draw_planes(std::list <Aircraft*> planes, Aircraft* selected) {
    std::list <Aircraft*> :: iterator plane = planes.begin();
    std::string color = "blue";

    while (plane != planes.end()) {
        color = "green";

        if (selected != NULL && (*plane) == selected) {
            color = "white";
        }

        this->draw_plane((*plane), color);

        ++plane;
    }
}

void Gameview::draw_airfield(Airfield* airfield) {
    std::vector <Runway> runways = airfield->get_runways();
	
    for (unsigned int i = 0; i < runways.size(); i+=2) {
		Point rwys = Tools::calculate(this->center_point, this->centerpoint, runways[i].get_start_place(), this->settings.zoom);
		Point rwye = Tools::calculate(this->center_point, this->centerpoint, runways[i].get_end_place(), this->settings.zoom);
		
        this->drawer.lineColor(rwys, rwye, "white");
    }
}

void Gameview::set_centerpoint(Coordinate& centerpoint) {
	this->centerpoint = centerpoint;
}

void Gameview::set_plane_progress(int handled, int required) {
	View::required = required;
	View::handled = handled;
}
