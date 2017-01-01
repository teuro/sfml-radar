#include "gamecontroller.hpp"

Gamecontroller::Gamecontroller(Settings& s, Drawsurface& d) : Controller(s, d) { 
	this->metar = new Metar;
	this->gameview = new Gameview(this->drawer, this->settings);
	this->game = new Game(this->settings);
	this->atis = new Atis(this->settings);
}

Gamecontroller::~Gamecontroller() { 
	delete this->metar;
	delete this->gameview;
	delete this->game;
	delete this->atis;
}

std::string Gamecontroller::handle_function_keys(int action) {
	if (action == Tools::RIGHT) {
		if (this->quicklist.size()) {
			this->command = this->quicklist.front();
		}
	}
	
	return this->command;
}

void Gamecontroller::handle_mouse_release(Point& mouse_start, Point& mouse_end) {
    double distance_px  = Tools::distancePX(mouse_start, mouse_end);
    double angle_rad    = Tools::angle(mouse_start, mouse_end);
    double distance_nm  = Tools::distanceNM(distance_px, this->settings.zoom);
	
    Coordinate center = Tools::calculate(this->game->get_centerpoint(), angle_rad, distance_nm);
	
	std::clog << "Game::get_centerpoint() " << this->game->get_centerpoint() << std::endl;
    this->game->set_centerpoint(center);
	std::clog << "Game::get_centerpoint() " << this->game->get_centerpoint() << std::endl;
}

void Gamecontroller::handle_mouse_wheel(int amount) {
    this->settings.zoom += (-amount * 10);

    if (this->settings.zoom < 10) {
        this->settings.zoom = 10;
    }
}

void Gamecontroller::update(double elapsed, bool draw) {
	this->game_time += elapsed;
	this->game->update(elapsed);
	this->metar->update("EFHK");
	
	if (draw) {
		this->gameview->repl["[PLH]"] = Tools::tostr(this->game->get_handled_planes());
		this->gameview->repl["[METAR]"] = this->metar->to_string();
		this->gameview->repl["[TIME]"] = Tools::totime(this->game_time, "H:i:s");
		this->gameview->repl["[PLC]"] = Tools::tostr(this->game->get_planes_count());
		this->gameview->repl["[SPE]"] = Tools::tostr(this->game->get_separation_errors());
		this->gameview->repl["[PLN]"] = Tools::totime(this->game->get_new_plane() - this->game_time, "i:s");
		this->gameview->repl["[SPK]"] = Tools::tostr(this->settings.max_separation_errors);
		this->gameview->repl["[RQD]"] = Tools::tostr(this->settings.required_handled);
		this->gameview->repl["[DEP]"] = Tools::tostr(this->atis->get_departure_runway());
		this->gameview->repl["[LND]"] = Tools::tostr(this->atis->get_landing_runway());
		this->gameview->repl["[TRL]"] = Tools::tostr(this->atis->get_transition_level());
		this->gameview->repl["[TRA]"] = Tools::tostr(this->atis->get_transition_altitude());
		
		this->gameview->set_centerpoint_map(this->game->get_centerpoint());
		this->gameview->clear_screen();
		this->gameview->add_element("Input", "input", "data", this->command);
		this->gameview->draw();
		this->gameview->draw_planes(this->game->get_aircrafts(), this->game->get_selected());
		this->gameview->draw_navpoints(this->game->get_navpoints());
		this->gameview->draw_airfield(this->game->get_active_field());
		this->gameview->render();
	}
}

void Gamecontroller::handle_mouse_click(Point& mouse) {
	this->game->select_aircraft(mouse);
}

void Gamecontroller::load() {
	std::clog << "Gamecontroller::load()" << std::endl;
	this->game->load("EFHK", this->atis->get_departure_runway(), this->atis->get_landing_runway());
	this->gameview->load();
}

void Gamecontroller::handle_text_input() {
    std::string t_command = this->command;
	this->game->build_clearance(command);
}

void Gamecontroller::update_command(std::string command) {
	this->command = command;
}

bool Gamecontroller::is_ok() {
    return false;
}
