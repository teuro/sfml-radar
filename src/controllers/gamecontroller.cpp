#include "gamecontroller.hpp"

Gamecontroller::Gamecontroller(Gameview& gv, Settings& s, Game& g, Atis& a) : Controller(s), gameview(gv), game(g), atis(a) { }

Gamecontroller::~Gamecontroller() { }

std::string Gamecontroller::handle_function_keys(int action) {
	if (action == Tools::RIGHT) {
		if (this->quicklist.size()) {
			this->command = this->quicklist.front();
		}
	}
	
	return this->command;
}

void Gamecontroller::handle_mouse_release(Point& mouse_start, Point& mouse_end) {
    double distance_px  = Tools::distancePX(mouse_end, mouse_start);
    double angle_rad    = Tools::angle(mouse_end, mouse_start) - (3.1415927 / 2.0);
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
		this->gameview.set_centerpoint(this->game.get_centerpoint());
		this->gameview.clear_screen();
		this->gameview.add_element("Metar", "metar", "data", this->game.get_metar());
		this->gameview.add_element("Input", "input", "data", this->command);
		this->gameview.draw();
		this->gameview.draw_planes(this->game.get_aircrafts(), this->game.get_selected());
		this->gameview.draw_navpoints(this->game.get_navpoints());
		this->gameview.draw_airfield(this->game.get_active_field());
		this->gameview.render();
	}
}

void Gamecontroller::handle_mouse_click(Point& mouse) {
	this->game.select_aircraft(mouse);
}

void Gamecontroller::load() {
	std::clog << "Gamecontroller::load()" << std::endl;
	this->game.load("EFHK", this->atis.get_departure_runway(), this->atis.get_landing_runway());
}

void Gamecontroller::handle_text_input() {
    std::string t_command = this->command;
}

void Gamecontroller::update_command(std::string command) {
	this->command = command;
}

bool Gamecontroller::is_ok() {
    return false;
}
