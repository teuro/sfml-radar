#include "game_view.hpp"

Gameview::Gameview(Drawsurface& d, Settings& s) : View(d), settings(s) { }

Gameview::~Gameview() { }

void Gameview::load() {
    this->center_point.set_place(this->settings.screen_width / 2, this->settings.screen_height/2);
    View::load();
}

void Gameview::draw() {
    //std::clog << "Gameview::draw()" << std::endl;
	
	this->View::draw();
}

void Gameview::draw_plane(Aircraft*& plane, Point& center_point, std::string color) {
	Point aircraft_place = Tools::calculate(this->center_point, this->settings.centerpoint, plane->get_place(), this->settings.zoom);
	
	double minute_px        = Tools::distancePX(plane->get_speed() * (1.0 / 60.0), this->settings.zoom, this->settings.screen_width);
    double separation_ring  = Tools::distancePX(settings.separation_horizontal / 2.0, this->settings.zoom, this->settings.screen_width);

    Point end_point = Tools::calculate(aircraft_place, plane->get_heading(), minute_px, true);
	
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
	
    for (unsigned int i = 0; i < runways.size(); i+=2) {
		Point rwys = Tools::calculate(this->center_point, this->settings.centerpoint, runways[i].get_start_place(), this->settings.zoom);
		Point rwye = Tools::calculate(this->center_point, this->settings.centerpoint, runways[i].get_end_place(), this->settings.zoom);
		
        this->drawer.lineColor(rwys, rwye, "white");
    }
}

void Gameview::draw_test() {
	Coordinate nor(this->settings.centerpoint.get_latitude()+0.150, this->settings.centerpoint.get_longitude());
	Coordinate sth(this->settings.centerpoint.get_latitude()-0.150, this->settings.centerpoint.get_longitude());
	Coordinate est(this->settings.centerpoint.get_latitude(), this->settings.centerpoint.get_longitude()+0.150);
	Coordinate wst(this->settings.centerpoint.get_latitude(), this->settings.centerpoint.get_longitude()-0.150);
	
	Point north = Tools::calculate(this->center_point, this->settings.centerpoint, nor, this->settings.zoom);
	Point south = Tools::calculate(this->center_point, this->settings.centerpoint, sth, this->settings.zoom);
	Point east = Tools::calculate(this->center_point, this->settings.centerpoint, est, this->settings.zoom);
	Point west = Tools::calculate(this->center_point, this->settings.centerpoint, wst, this->settings.zoom);
	
	//drawer.rectangleColor(north, 10, "red");
	//drawer.rectangleColor(south, 10, "red");
	drawer.rectangleColor(east, 10, "red");
	//drawer.rectangleColor(west, 10, "red");
}
