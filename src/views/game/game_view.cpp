#include "game_view.hpp"

Gameview::Gameview(Drawsurface& d, Settings& s) : View(d, s) { }

Gameview::~Gameview() { }

void Gameview::load() {
	std::clog << "Gameview::load()" << std::endl;
	
	View::load();
	
	std::ifstream in_file("templates/plane.tpl", std::ios::in);
	std::string name;
	int x;
	int y;
	
	while (in_file >> name >> x >> y) {
		Point tmp(x, y);
		name = Tools::trim(name);
				
		if (name == "CALLSIGN") {
			this->text_callsign = tmp;
		} else if (name == "SPEED") {
			this->text_speed = tmp;
		} else if (name == "ALTITUDE") {
			this->text_altitude = tmp;
		} else if (name == "HEADING") {
			this->text_heading = tmp;
		}
	}
	
	in_file.close();
}

void Gameview::draw() {
	this->centerpoint_screen.set_place(this->settings.screen_width / 2, this->settings.screen_height/2);
	this->View::draw();
}

void Gameview::draw_plane(Aircraft*& plane, std::string color) {
	Point aircraft_place = Tools::calculate(plane->get_place(), this->settings.zoom);
	Point draw;

    double separation_ring = Tools::distancePX(settings.separation_horizontal / 2.0, this->settings.zoom);
	
	Point end_point_c = Tools::calculate(aircraft_place, Tools::CalcGeograpicAngle(plane->get_heading()), Tools::distancePX(plane->get_speed() * (1.0 / 60.0), this->settings.zoom));
	
    drawer.lineColor(aircraft_place, end_point_c, color);
    drawer.circleColor(aircraft_place, separation_ring, color);
    drawer.rectangleColor(aircraft_place, 10, color);
	
	draw = this->text_callsign + aircraft_place;
    drawer.draw_text(plane->get_name(), draw, color);
	
	draw = this->text_speed + aircraft_place;
    drawer.draw_text(Tools::tostr(plane->get_speed()), draw, color);
	
	draw = this->text_heading + aircraft_place;
    drawer.draw_text(Tools::tostr(Tools::rad2deg(plane->get_heading())), draw, color);
	
	draw = this->text_altitude + aircraft_place;
    drawer.draw_text(Tools::tostr(plane->get_altitude()), draw, color);
}

void Gameview::draw_navpoints(std::vector <Navpoint>& navpoints) {
    for (unsigned int i = 0; i < navpoints.size(); ++i) {
        Point place_screen = Tools::calculate(navpoints[i].get_place(), this->settings.zoom);
		std::clog << navpoints[i].get_name() << " " << navpoints[i].get_place() << " => " << place_screen << std::endl;

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
		Point rwys = Tools::calculate(runways[i].get_start_place(), this->settings.zoom);
		Point rwye = Tools::calculate(runways[i].get_end_place(), this->settings.zoom);
		
        this->drawer.lineColor(rwys, rwye, "white");
    }
}

void Gameview::set_centerpoint_map(Coordinate& centerpoint_map) {
	this->centerpoint_map = centerpoint_map;
}