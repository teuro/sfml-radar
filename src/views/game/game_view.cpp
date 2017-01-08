#include "game_view.hpp"

double min_lat = 59.5;
double max_lat = 61.5;
double min_lon = 23.0;
double max_lon = 26.0;

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
	Point aircraft_place = this->calculate(plane->get_place());
	Point draw;

    Coordinate separation_ring_place_c = Tools::calculate(plane->get_place(), settings.separation_horizontal / 2.0, 1);
	Point separation_ring_place_p = this->calculate(separation_ring_place_c);
	double separation_ring = Tools::distancePX(aircraft_place, separation_ring_place_p);
	
	Coordinate end_point_place_c = Tools::calculate(plane->get_place(), plane->get_heading(), plane->get_speed() * (1.0 / 60.0));
	Point end_point_place_p = this->calculate(end_point_place_c);
	
    drawer.lineColor(aircraft_place, end_point_place_p, color);
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
        Point place_screen = this->calculate(navpoints[i].get_place());
		
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
		Point rwys = this->calculate(runways[i].get_start_place());
		Point rwye = this->calculate(runways[i].get_end_place());
		
        this->drawer.lineColor(rwys, rwye, "white");
    }
}

void Gameview::set_zoom(int zoom) {
	Coordinate a(min_lat, min_lon);
	Coordinate b(max_lat, max_lon);
	
	double distance = zoom / 2.0;
	
	Coordinate c(Tools::calculate(this->centerpoint_map, Tools::deg2rad(315.0), std::sqrt(std::pow(distance, 2.0) + std::pow(distance, 2.0))));
	Coordinate d(Tools::calculate(this->centerpoint_map, Tools::deg2rad(135.0), std::sqrt(std::pow(distance, 2.0) + std::pow(distance, 2.0))));
	
	min_lat = d.get_latitude();
	max_lat = c.get_latitude();
	
	min_lon = c.get_longitude();
	max_lon = d.get_longitude();
}

void Gameview::set_centerpoint_map(Coordinate& centerpoint_map) {
	double distanceNM = Tools::distanceNM(centerpoint_map, this->centerpoint_map);
	double angle_rad = Tools::CalcGeograpicAngle(Tools::fix_angle(Tools::angle(centerpoint_map, this->centerpoint_map) + Tools::get_PI()));
	
	Coordinate a(min_lat, min_lon);
	Coordinate b(max_lat, max_lon);
	
	if (distanceNM > 0.0) {
		a = Tools::calculate(a, angle_rad, distanceNM);
		b = Tools::calculate(b, angle_rad, distanceNM);
	}
	
	min_lat = a.get_latitude();
	min_lon = a.get_longitude();
	max_lat = b.get_latitude();
	max_lon = b.get_longitude();
	
	this->centerpoint_map = centerpoint_map;
}

Point Gameview::calculate(Coordinate& target) {
	int pixelY = this->settings.screen_height - ((target.get_latitude() - min_lat) / (max_lat - min_lat)) * this->settings.screen_height;
	int pixelX = ((target.get_longitude() - min_lon) / (max_lon - min_lon)) * this->settings.screen_width;
	
	Point t(pixelX, pixelY);
	
	return t;
}