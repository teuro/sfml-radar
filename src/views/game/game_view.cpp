#include "game_view.hpp"

double min_lat = 59.5;
double max_lat = 61.5;
double min_lon = 23.0;
double max_lon = 26.0;

Drawable_plane::Drawable_plane(std::string call, std::string t_name, std::string t_class, std::string t_id) : Drawable_element(t_name, t_class, t_id), callsign(call) { }

std::string Drawable_plane::get_callsign() {
	return this->callsign;
}
	
void Drawable_plane::set_callsign(std::string call) {
	this->callsign = call;
}

Gameview::Gameview(Drawsurface& d, Settings& s) : View(d, s) { }

Gameview::~Gameview() { }

void Gameview::load() {
	std::clog << "Gameview::load()" << std::endl;
	
	View::load("game");
}

void Gameview::update_command(std::string t_command) {
	this->command = t_command;
}

void Gameview::draw() {
	this->centerpoint_screen.set_place(this->settings.screen_width / 2, this->settings.screen_height / 2);
	this->inputs.back().set_value(this->command);
	this->View::draw();
	
	for (unsigned int i = 0; i < this->lists.size(); ++i) {
		this->style(this->lists[i]);
		this->draw_element(this->lists[i]);
	}
}

void Gameview::draw_plane(Aircraft*& plane, Aircraft* selected, Point& mouse) {
	lists.clear();
	
	Point aircraft_place = this->calculate(plane->get_place());
	Point draw;
	std::string id = "";

    Coordinate separation_ring_place_c = Tools::calculate(plane->get_place(), settings.separation_horizontal / 2.0, 1);
	Point separation_ring_place_p = this->calculate(separation_ring_place_c);
	double separation_ring = Tools::distancePX(aircraft_place, separation_ring_place_p);
	
	Coordinate end_point_place_c = Tools::calculate(plane->get_place(), plane->get_heading(), plane->get_speed() * (1.0 / 60.0));
	Point end_point_place_p = this->calculate(end_point_place_c);
	
	if (plane == selected) {
		id = "selected";
	}
	
	Drawable_plane dplane("plane", "plane", "", id);
	Drawable_list info_list("ul", "infolist", id);
	
	info_list.add_element(plane->get_name());
	
	if (id == "selected") {
		info_list.add_element(Tools::tostr((int)plane->get_speed()));
		info_list.add_element(Tools::tostr((int)Tools::rad2deg(plane->get_heading())));
		info_list.add_element(Tools::tostr((int)plane->get_altitude()));
		
		if (plane->get_type() == 0) {
			info_list.add_element(plane->get_target().get_name());
		}
		
		this->drawer.lineColor(aircraft_place, mouse, 250, 60, 60);
		double heading = Tools::angle(aircraft_place, mouse);
		
		heading = Tools::fix_angle(heading - Tools::get_PI() / 2.0);
		heading = Tools::rad2deg(heading);
	
		Point text_place = Tools::calculate_midpoint(aircraft_place, mouse);
		
		this->drawer.draw_text(Tools::tostr(heading), text_place, 250,60 ,60);
	}
	
	this->style(dplane);
	this->style(info_list);
	
	info_list.get_style().set_place(aircraft_place);
	
	drawer.lineColor(aircraft_place, end_point_place_p, dplane.get_style().get_text_color());
    drawer.circleColor(aircraft_place, separation_ring, dplane.get_style().get_text_color());
	
	this->draw_element(info_list);
}

void Gameview::draw_navpoints(std::vector <Navpoint>& navpoints) {
    for (unsigned int i = 0; i < navpoints.size(); ++i) {
        Point place_screen = this->calculate(navpoints[i].get_place());
		
        this->drawer.trigonColor(place_screen, 15, 15245785);
		place_screen.change_x(40);
		this->drawer.draw_text(navpoints[i].get_name(), place_screen, 15264587);
    }
}

void Gameview::draw_planes(std::list <Aircraft*> planes, Aircraft* selected, Point& mouse) {
    std::list <Aircraft*> :: iterator plane = planes.begin();
    	
    while (plane != planes.end()) {
        this->draw_plane((*plane), selected, mouse);
        ++plane;
    }
}

void Gameview::draw_airfield(Airfield* airfield) {
    std::vector <Runway> runways = airfield->get_runways();
	
    for (unsigned int i = 0; i < runways.size(); i+=2) {
		Point rwys = this->calculate(runways[i].get_start_place());
		Point rwye = this->calculate(runways[i].get_end_place());
		
        this->drawer.lineColor(rwys, rwye, 1524875);
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