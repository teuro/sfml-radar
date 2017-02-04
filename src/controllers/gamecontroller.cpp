#include "gamecontroller.hpp"

Gamecontroller::Gamecontroller(Settings& s, Drawsurface& d) : Controller(s, d) { 
	this->metar = new Metar;
	this->gameview = new Gameview(this->drawer, this->settings);
	this->game = new Game(this->settings);
	this->atis = new Atis(this->settings);
	this->atisview = new Atisview(this->drawer, this->settings, this->atis);
	this->settings.zoom = 110;
	this->frames = 0;
	this->fps_time = 5000;
	this->state = ATIS;
}

Gamecontroller::~Gamecontroller() { 
	delete this->metar;
	delete this->gameview;
	delete this->atisview;
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
    double distance_nm  = 7;
	
	if (distance_px == 0) {
		distance_nm = 0;
	}
	
    Coordinate center = Tools::calculate(this->game->get_centerpoint(), angle_rad, distance_nm);
	
	this->game->set_centerpoint(center);
}

void Gamecontroller::handle_mouse_wheel(int amount) {
    this->settings.zoom += (-amount * 10);

    if (this->settings.zoom < 30) {
        this->settings.zoom = 30;
    } else if (this->settings.zoom > 110) {
		this->settings.zoom = 110;
	}
	
	this->gameview->set_zoom(this->settings.zoom);
}

void Gamecontroller::set_variables() {
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
	this->gameview->repl["[FPS]"] = Tools::tostr(this->fps);
	this->gameview->repl["[CLRC]"] = Tools::tostr(this->clearances.size());
	
	this->atisview->repl["[METAR]"] = this->metar->to_string();
	this->atisview->repl["[DEPARTURE]"] = this->atis->get_departure_runway();
	this->atisview->repl["[LANDING]"] = this->atis->get_landing_runway();
	this->atisview->repl["[LEVEL]"] = Tools::tostr(this->atis->get_transition_level());
	this->atisview->repl["[ALTITUDE]"] = Tools::tostr(this->atis->get_transition_altitude());
}

void Gamecontroller::calculate_fps() {
	if (this->game_time > this->fps_end_time) {
		this->fps = this->frames / (fps_time / 1000.0);
		this->fps_end_time += this->fps_time;
		this->frames = 0;
	}
}

void Gamecontroller::draw_logic() {
	++this->frames;
	
	if (this->state == GAME) {	
		this->gameview->set_centerpoint_map(this->game->get_centerpoint());
		this->gameview->clear_screen();
		this->gameview->update_command(this->command);
		this->gameview->draw();
		this->gameview->draw_planes(this->game->get_aircrafts(), this->game->get_selected());
		this->gameview->draw_navpoints(this->game->get_navpoints());
		this->gameview->draw_airfield(this->game->get_active_field());
		this->gameview->render();
	} else if (this->state == ATIS) {
		this->atisview->clear_screen();
		this->atisview->draw();
		this->atisview->draw_runways(this->game->get_active_field()->get_runways());
		this->atisview->render();
	}
}

void Gamecontroller::update(double elapsed) {
	this->set_variables();
	
	if (this->state == GAME) {
		this->game_time += elapsed;
		
		this->calculate_fps();
		
		this->game->update(elapsed);
		this->metar->update(this->game->get_active_field()->get_name());
		/**
			* @todo check if handled_planes >= required_planes and plane_list is empty
		**/
	}
	
	this->draw_logic();
}

void Gamecontroller::handle_mouse_click(Point& mouse) {
	if (this->state == GAME) {
		this->game->selected = NULL;

		std::list <Aircraft*> :: iterator plane;
		std::list <Aircraft*> aircrafts = this->game->get_aircrafts();

		for (plane = aircrafts.begin(); plane != aircrafts.end(); ++plane) {
			Point aircraft = this->gameview->calculate((*plane)->get_place());

			if (Tools::on_area(mouse, aircraft)) {
				this->game->selected = (*plane);
			}
		}
	} else if (this->state == ATIS) {
		std::string type 	= this->atisview->get_type(mouse);
		std::string runway 	= this->atisview->get_runway(this->game->get_active_field()->get_runways(), mouse);
		int level			= this->atisview->get_level(mouse);
		int altitude 		= this->atisview->get_altitude(mouse);
		
		//std::clog << "type = " << type << std::endl;
		
		if (type == "departure") {
			this->atis->set_departure_runway(runway);
		} else if (type == "landing") {
			this->atis->set_landing_runway(runway);
		} else if (type == "level") {
			this->atis->set_transition_level(level);
		} else if (type == "altitude") {
			this->atis->set_transition_altitude(altitude);
		}
	}
}

void Gamecontroller::load() {
	std::clog << "Gamecontroller::load()" << std::endl;
	this->game->load("EFHK", this->atis->get_departure_runway(), this->atis->get_landing_runway());
	this->gameview->load();
	this->atisview->load();
}

void Gamecontroller::handle_text_input() {
    std::string t_command = this->command;
	
	if (t_command == "reset") {
		this->game->set_centerpoint(this->settings.centerpoint);
		return;
	}
	
	this->game->build_clearance(command);
	Clearance t_clearance = {this->game_time, this->game->get_selected()->get_name(), this->command};
	this->clearances.push_back(t_clearance);
}

void Gamecontroller::update_command(std::string command) {
	this->command = command;
}

bool Gamecontroller::is_ok() {
    return false;
}
