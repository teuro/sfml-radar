#include "game_view.hpp"

Drawable_plane::Drawable_plane(std::string call, std::string t_name, std::string t_class, std::string t_id) : Drawable_element(t_name, t_class, t_id), callsign(call) { 
	
}

std::string Drawable_plane::get_callsign() {
	return this->callsign;
}
	
void Drawable_plane::set_callsign(std::string call) {
	this->callsign = call;
}

Gameview::Gameview(Drawsurface& d, std::shared_ptr <Settings> s, std::shared_ptr <Game> g) : View(d, s), game(g) { 
	this->loaded = false;
}

Gameview::~Gameview() { }

void Gameview::load() {
	std::clog << "Gameview::load()" << std::endl;
	this->loaded = true;
	View::load("game");
}

void Gameview::update_command(std::string t_command) {
	this->command = t_command;
}

std::string Gameview::handle_click(Point& mouse) {
	this->game->selected = NULL;
	
	std::list <Aircraft*> :: iterator plane;
	std::list <Aircraft*> aircrafts = this->game->get_aircrafts();

	for (plane = aircrafts.begin(); plane != aircrafts.end(); ++plane) {
		Point aircraft = this->calculate((*plane)->get_place());

		if (Tools::on_area(mouse, aircraft, 10)) {
			this->game->selected = (*plane);
			return "Plane " + (*plane)->get_name() + " selected";
		}
	}
	
	return "";
}

void Gameview::draw(Point& mouse) {
	this->clear_screen();
	
	if (!this->loaded) {
		throw std::logic_error("Gameview is not loaded");
	}
	
	this->centerpoint_screen.set_place(this->settings->screen_width / 2, this->settings->screen_height / 2);
	this->inputs.back().set_value(this->command);
	
	this->calculate_coordinate_limits(this->settings->zoom);
	this->update_command(this->command);
	
	this->View::draw();
	
	this->draw_airfield(this->game->get_active_field());
	this->draw_planes(this->game->get_aircrafts(), this->game->get_selected(), mouse);
}

void Gameview::draw_plane(Aircraft*& plane, Aircraft* selected, Point& mouse) {
	Point aircraft_place = this->calculate(plane->get_place());
	Point draw;
	std::string id = "";

    Coordinate separation_ring_place_c = Tools::calculate(plane->get_place(), this->settings->separation_horizontal / 2.0, 1);
	Point separation_ring_place_p = this->calculate(separation_ring_place_c);
	double separation_ring = Tools::distancePX(aircraft_place, separation_ring_place_p);
	
	Coordinate end_point_place_c = Tools::calculate(plane->get_place(), plane->get_heading(), plane->get_speed() * (1.0 / 60.0));
	Point end_point_place_p = this->calculate(end_point_place_c);
	
	if (plane == selected) {
		id = "selected";
	}
	
	Drawable_list info_list("ul", "infolist", id);
	
	info_list.add_element(plane->get_name());
	info_list.add_element(Tools::tostr((int)plane->get_altitude()) + " / " + Tools::tostr(plane->get_clearance_altitude()));
	info_list.set_class("normal");
	
	if (id == "selected") {
		info_list.set_class("active");
		this->style(info_list);
		
		info_list.add_element(Tools::tostr((int)plane->get_speed()) + " / " + Tools::tostr((int)plane->get_clearance_speed()));
		info_list.add_element(Tools::tostr((int)Tools::rad2deg(plane->get_heading())) + " / " + Tools::tostr((int)Tools::rad2deg(plane->get_clearance_heading())));
		
		if (plane->get_type() == DEPARTURE) {
			info_list.add_element(plane->get_target().get_name());
		}
		
		double heading = Tools::angle(aircraft_place, mouse);
		double distance = Tools::distancePX(aircraft_place, mouse);
		
		heading = Tools::fix_angle(heading - Tools::get_PI() / 2.0);
		heading = Tools::rad2deg(heading);
		distance = this->distanceNM(distance);
	
		Point text_place = Tools::calculate_midpoint(aircraft_place, mouse);
		
		this->drawer.draw_text(Tools::tostr((int)heading) + " deg " + Tools::tostr((int)distance) + " nm", text_place, info_list.get_style().get_text_color());
		this->drawer.lineColor(aircraft_place, mouse, info_list.get_style().get_text_color());
	}
	
	this->style(info_list);
	
	info_list.get_style().set_place(aircraft_place);
	
	drawer.lineColor(aircraft_place, end_point_place_p, info_list.get_style().get_text_color());
    drawer.circleColor(aircraft_place, separation_ring, info_list.get_style().get_text_color());
	
	this->draw_element(info_list);
}

void Gameview::draw_planes(std::list <Aircraft*> planes, Aircraft* selected, Point& mouse) {
    std::list <Aircraft*> :: iterator plane = planes.begin();
	Drawable_table plane_table("table", "", "planelist");
    	
    while (plane != planes.end()) {
		std::string special;
		Row row("tr", "normal", "");
		
		if ((*plane) == selected) {
			row.set_class("selected");
		}
		
		this->style(row);
		plane_table.add_row(row);
		
		if ((*plane)->get_type() == APPROACH) {
			if ((*plane)->get_expect() && !(*plane)->get_approach()) {
				special = "E " + (*plane)->get_landing_runway_name();
			} else if ((*plane)->get_approach()) {
				special = "A " + (*plane)->get_landing_runway_name();
			} else if ((*plane)->get_landing()) {
				special = "L " + (*plane)->get_landing_runway_name();
			}
		} else if ((*plane)->get_direct()) {
			special = "D " + (*plane)->get_target_name();
		}
		
		Cell cell_1((*plane)->get_name());
		plane_table.add_cell(cell_1);
		
		Cell cell_2(special);
		plane_table.add_cell(cell_2);

        this->draw_plane((*plane), selected, mouse);
        ++plane;
    }
	
	this->style(plane_table);
	this->draw_element(plane_table);
}

double Gameview::distancePX(double nautical) {
	double center_w = this->settings->screen_width / 2.0;
	double center_h = this->settings->screen_height / 2.0;
	
	double center2corner = std::sqrt(std::pow(center_w, 2.0) + std::pow(center_h, 2.0));
	
	return (nautical * center2corner) / this->settings->zoom;
}

void Gameview::draw_airfield(std::shared_ptr <Airfield> airfield) {
    std::vector <Runway> runways = airfield->get_runways();
    std::vector <Navpoint> navpoints = airfield->get_navpoints();
	int color = 1524875;
	
    for (unsigned int i = 0; i < runways.size(); ++i) {
		Point rwys = this->calculate(runways[i].get_start_place());
		Point rwye = this->calculate(runways[i].get_end_place());
		Point rwya = this->calculate(runways[i].get_approach_place());
		
        this->drawer.lineColor(rwys, rwye, color);
        this->drawer.circleColor(rwya, 5, color);
    }
	
	for (unsigned int i = 0; i < navpoints.size(); ++i) {
        Point place_screen = this->calculate(navpoints[i].get_place());
		
        this->drawer.trigonColor(place_screen, 15, color);
		place_screen.change_x(15);
		place_screen.change_y(-10);
		this->drawer.draw_text(navpoints[i].get_name(), place_screen, color);
    }
}

Point Gameview::calculate(Coordinate& target) {
	int pixelY = this->settings->screen_height - ((target.get_latitude() - this->min_lat) / (this->max_lat - this->min_lat)) * this->settings->screen_height;
	int pixelX = ((target.get_longitude() - this->min_lon) / (this->max_lon - this->min_lon)) * this->settings->screen_width;
	
	Point t(pixelX, pixelY);
	
	return t;
}

void Gameview::update() { }
