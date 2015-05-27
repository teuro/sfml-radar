#include "gamecontroller.hpp"

Gamecontroller::Gamecontroller(Gameview& gv, Settings& s, Game& g) : Controller(s), gameview(gv), game(g) {
    this->function_key = Tools::SPEED;
}

Gamecontroller::~Gamecontroller() { }

void Gamecontroller::handle_function_keys(int action) {
    this->function_key = action;
}

void Gamecontroller::handle_mouse_release(Point& mouse_start, Point& mouse_end) {
    double distance_px  = Tools::distancePX(mouse_end, mouse_start);
    double angle_rad    = Tools::angle(mouse_end, mouse_start);
    double distance_nm  = Tools::distanceNM(distance_px, this->settings.zoom, this->settings.screen_width);

    Coordinate center = Tools::calculate(this->game.get_centerpoint(), angle_rad, distance_nm);

    this->game.set_centerpoint(center);
}

void Gamecontroller::handle_mouse_wheel(int amount) {
    this->settings.zoom += (-amount * 10);

    if (this->settings.zoom < 10) {
        this->settings.zoom = 10;
    }
}

void Gamecontroller::update(double elapsed, bool draw) {
    this->game.update(elapsed);

    if (draw) {
        this->gameview.load();
        this->gameview.clear_screen();
        this->gameview.add_element("Metar", "metar", "data", this->game.get_metar().get_metar());
        //this->gameview.add_element("Atisbox", "atis-box", "data", this->game.get_departure());
        this->gameview.add_element("Atisbox", "atis-box", "data", this->game.get_landing());
        //this->gameview.add_element("Atisbox", "atis-box", "data", this->game.get_transition_altitude());
        //this->gameview.add_element("Atisbox", "atis-box", "data", this->game.get_transition_level());
        this->gameview.draw();
        this->gameview.draw_planes(this->game.get_aircrafts(), this->game.get_selected());
        this->gameview.draw_navpoints(this->game.get_navpoints());
        this->gameview.draw_airfield(this->game.get_active_field());
        this->gameview.render();
    }

}

void Gamecontroller::set_centerpoint(Coordinate& cp) {
    this->game.set_centerpoint(cp);
}

Coordinate& Gamecontroller::get_centerpoint() {
    return this->game.get_centerpoint();
}

void Gamecontroller::handle_mouse_click(Point& mouse) {
    this->game.select_aircraft(mouse);
}

void Gamecontroller::load() {
    std::clog << "Gamecontroller::load()" << std::endl;
    this->game.load();
}

void Gamecontroller::handle_text_input() {
    std::string command = this->game.get_command();
    std::string callsign;
    std::string type;

    callsign = command.substr(0, command.find(":"));
    command = command.substr(command.find(": ")+1);
    command = Tools::trim(command);

    std::vector <std::string> parts = Tools::split(" ", command);

    this->game.set_clearance(callsign, parts);
    parts.erase(parts.begin(), parts.end());
}

std::string Gamecontroller::matching_elements(std::string input) {
	std::clog << "Gamecontroller::matching_elements(" << input << ")" << std::endl;
	std::string elements = "";
	std::list <Aircraft*> t_planes = this->game.get_aircrafts();
	
	std::list <Aircraft*> :: iterator plane_a = t_planes.begin();
	
	while (plane_a != t_planes.end()) {
		//std::clog << (*plane_a)->get_name() << std::endl;
		if (Tools::is_match(input, (*plane_a)->get_name())) {
			elements += (*plane_a)->get_name() + " ";
		}
		++plane_a;
	}
		
	return elements;
}

void Gamecontroller::update_command(std::string command) {
	std::clog << this->matching_elements(command) << std::endl;
    this->game.set_command("");
    this->game.set_command(command);
}

bool Gamecontroller::is_ok() {
    return true;
}

Airfield* Gamecontroller::get_active_field() {
    return this->game.get_active_field();
}
