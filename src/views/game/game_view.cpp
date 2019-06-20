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
	#ifdef DEBUG
	std::clog << "Gameview::load()" << std::endl;
	#endif
	
	this->loaded = true;
	View::load("game");
	
	std::shared_ptr <Drawable_input> di(new Drawable_input("", "input", "userinput", "userinput"));
	this->input = di; 
}

std::string Gameview::handle_click(Point& mouse) {
	this->game->selected = NULL;
	
	std::list <aircraft> :: iterator plane;
	std::list <aircraft> aircrafts = this->game->get_aircrafts();

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
	
	this->calculate_coordinate_limits();
	
	this->draw_airfield(this->game->get_active_field());
	this->draw_planes(this->game->get_aircrafts(), this->game->get_selected(), mouse);
	this->draw_holdings();
	
	style(this->input);
	draw_element(this->input);
	
	this->View::draw();
}

void Gameview::draw_plane(aircraft plane, aircraft selected, Point& mouse) {
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
	
	Drawable_list info_list("ul", "", id);
	
	info_list.add_element(plane->get_name());
	info_list.add_element(Tools::tostr(plane->get_altitude()) + " / " + Tools::tostr(plane->get_clearance_altitude()));
	
	info_list.set_class("normal");
	
	if (id == "selected") {
		info_list.add_element(Tools::tostr(plane->get_speed()) + " / " + Tools::tostr(plane->get_clearance_speed()));
		info_list.add_element(Tools::tostr(Tools::rad2deg(plane->get_heading())) + " / " + Tools::tostr(Tools::rad2deg(plane->get_clearance_heading())));
		
		if (plane->get_type() == DEPARTURE) {
			info_list.add_element(plane->get_target().get_name());
		}
		
		info_list.clear_classes();
		info_list.set_class("active");
		
		this->style(info_list);
		
		Coordinate c_plane = plane->get_place();
		Coordinate c_mouse = this->calculate(mouse);
		
		double heading = Tools::angle(c_plane, c_mouse);
		double distance = Tools::distancePX(aircraft_place, mouse);
		
		heading = Tools::fix_angle(heading);
		
		heading = Tools::rad2deg(heading);
		distance = this->distanceNM(distance);
	
		Point text_place = Tools::calculate_midpoint(aircraft_place, mouse);
		
		this->drawer.draw_text(Tools::tostr(heading) + " deg " + Tools::tostr(distance) + " nm", text_place, info_list.get_style().get_text_color());
		this->drawer.lineColor(aircraft_place, mouse, info_list.get_style().get_text_color());
	}
	
	Point info_list_place(aircraft_place.get_x() + 20, aircraft_place.get_y() + 20);
	
	info_list.get_style().set_place(info_list_place);
	
	this->draw_element(info_list);
	
	drawer.lineColor(aircraft_place, end_point_place_p, info_list.get_style().get_text_color());
    drawer.circleColor(aircraft_place, separation_ring, info_list.get_style().get_text_color());
}

void Gameview::draw_holdings() {
	Drawable_list holding_list("ul", "holdings", "");
	
	std::list <aircraft> holdings = this->game->get_holdings();
	std::list <aircraft> :: iterator holding = holdings.begin();
	
	holding_list.add_element("Holdings");
	
	while (holding != holdings.end()) {
		holding_list.add_element((*holding)->get_name());
		
		++holding;
	}
	
	this->draw_element(holding_list);
}

void Gameview::draw_planes(std::list <aircraft> planes, aircraft selected, Point& mouse) {
    std::list <aircraft> :: iterator plane = planes.begin();
	Drawable_table plane_table("table", "normal", "planelist");
    	
    while (plane != planes.end()) {
		std::string special;
		Row row("tr", "", "");
		
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
		
		Cell cell_2(special);
		
		if ((*plane) == selected) {
			row.clear_classes();
			row.set_class("active");
			cell_1.set_class("active");
			cell_2.set_class("active");
		} else {
			row.clear_classes();
			row.set_class("normal");
			cell_1.set_class("normal");
			cell_2.set_class("normal");
		}
		
		row.add_cell(cell_1);
		row.add_cell(cell_2);
		
		plane_table.add_row(row);

        this->draw_plane((*plane), selected, mouse);
        ++plane;
    }

	this->draw_element(plane_table);
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
	this->calculate_coordinate_limits();
	
	int pixelY = this->settings->screen_height - ((target.get_latitude() - this->min_lat) / (this->max_lat - this->min_lat)) * this->settings->screen_height;
	int pixelX = ((target.get_longitude() - this->min_lon) / (this->max_lon - this->min_lon)) * this->settings->screen_width;
	
	Point t(pixelX, pixelY);
	
	return t;
}

Coordinate Gameview::calculate(Point& target) {
	double t_latitude = (1.0 - ((double)target.get_y() / this->settings->screen_height)) * (max_lat - min_lat) + min_lat;
	double t_longitude = ((double)target.get_x() / this->settings->screen_width) * (max_lon - min_lon) + min_lon;
	
	Coordinate t(t_latitude, t_longitude);

	return t;
}

void Gameview::update() { }

void Gameview::update_command(std::string command) {
	this->input->set_value(command);
}
