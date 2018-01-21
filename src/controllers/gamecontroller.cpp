#include "gamecontroller.hpp"

Gamecontroller::Gamecontroller(Settings& s, Drawsurface& d) : Controller(s, d) { 
	Metar metar;
	this->gameview = new Gameview(this->drawer, this->settings);
	this->atis = new Atis(this->settings, this->metar);
	this->game = new Game(this->settings, this->atis);
	
	this->settings.zoom = 110;
	this->frames = 0;
	this->fps_time = 5000;
	this->fps_end_time = 8500;
	this->state = MENU;
	this->flash_message_begin = -1;
	this->flash_message_time = 2500;
	quick_iterator = this->quicklist.begin();
}

Gamecontroller::~Gamecontroller() { 
	delete this->gameview;
	delete this->atisview;
	delete this->statview;
	delete this->menuview;
	delete this->game;
	delete this->atis;
	//delete this->menu;
}

void Gamecontroller::load() {
	std::clog << "Gamecontroller::load()" << std::endl;
	
	this->airports 	= new Menu("airport", "airport");
	this->menu 		= new Menu("airport", "airport");
	
	this->load_menu_items("SELECT ICAO FROM airfields", this->airports);
	this->menu = this->airports;
	
	this->atisview 	= new Atisview(this->drawer, this->settings, this->atis);
	this->statview 	= new Statview(this->drawer, this->settings);
	this->menuview 	= new Menuview(this->drawer, this->settings, this->menu);
	
	this->menuview->load();
	this->atisview->load();
	this->state = MENU;
}

std::string Gamecontroller::handle_function_keys() {
	return this->command;
}

void Gamecontroller::handle_mouse_release(Point& mouse_start, Point& mouse_end) {
    double distance_px  = Tools::distancePX(mouse_start, mouse_end);
    double angle_rad    = Tools::fix_angle(Tools::angle(mouse_start, mouse_end) + Tools::get_PI() / 2.0);
    double distance_nm  = this->gameview->distanceNM(distance_px);
	
    this->settings.centerpoint = Tools::calculate(this->settings.centerpoint, angle_rad, distance_nm);
}

void Gamecontroller::handle_mouse_wheel(int amount) {
    if (this->state == GAME) {
		this->settings.zoom += (-amount * 1);
		
		if (this->settings.zoom < this->settings.zoom_min) {
			this->settings.zoom = this->settings.zoom_min;
		} else if (this->settings.zoom > this->settings.zoom_max) {
			this->settings.zoom = this->settings.zoom_max;
		}
		
		this->gameview->set_zoom(this->settings.zoom);
	} else if (this->state == MENU) {
		this->menu->change_selection(-amount);
	} else if (this->state == ATIS) {
		this->atis->update(-amount);
	}
}

void Gamecontroller::set_variables() {
	this->menuview->repl["[MPL]"] = Tools::tostr(this->settings.max_planes);
	this->menuview->repl["[RQD]"] = Tools::tostr(this->settings.required_handled);
	this->menuview->repl["[LEV]"] = Tools::tostr(this->game->get_level());
	
	this->gameview->repl["[PLH]"] = Tools::tostr(this->game->get_handled_planes());
	this->gameview->repl["[METAR]"] = this->metar.to_string();
	this->gameview->repl["[TIME]"] = Tools::totime(this->game_time, "H:i:s");
	this->gameview->repl["[PLC]"] = Tools::tostr(this->game->get_planes_count());
	this->gameview->repl["[SPE]"] = Tools::tostr(this->game->get_separation_errors());
	if (this->game->get_new_plane() > this->game_time) {
		this->gameview->repl["[PLN]"] = Tools::totime(this->game->get_new_plane() - this->game_time, "i:s");
	} else {
		this->gameview->repl["[PLN]"] = "no plane";
	}
	this->gameview->repl["[SPK]"] = Tools::tostr(this->settings.max_separation_errors);
	this->gameview->repl["[RQD]"] = Tools::tostr(this->settings.required_handled);
	this->gameview->repl["[DEP]"] = Tools::tostr(this->atis->get_departure_runway().get_name());
	this->gameview->repl["[LND]"] = Tools::tostr(this->atis->get_landing_runway().get_name());
	this->gameview->repl["[TRL]"] = Tools::tostr(this->atis->get_transition_level());
	this->gameview->repl["[TRA]"] = Tools::tostr(this->atis->get_transition_altitude());
	this->gameview->repl["[FPS]"] = Tools::tostr(this->fps);
	this->gameview->repl["[CLRC]"] = Tools::tostr(this->game->get_clearances().size());
	this->gameview->repl["[PCNT]"] = Tools::tostr(this->game->get_game_points());
	this->gameview->repl["[CLRE]"] = Tools::tostr(this->game->get_clearance_error());
	this->gameview->repl["[GRE]"] = Tools::tostr(this->game->get_game_error());
	this->gameview->repl["[MXA]"] = Tools::tostr(Tools::round_nearest(Tools::rad2deg(this->game->get_landing_runway().get_heading()) + this->settings.approach_angle, 10));
	this->gameview->repl["[MNA]"] = Tools::tostr(Tools::round_nearest(Tools::rad2deg(this->game->get_landing_runway().get_heading()) - this->settings.approach_angle, 10));
	
	if (this->game->get_selected() != NULL) {
		this->gameview->repl["[PLNA]"] = Tools::tostr(Tools::rad2deg(this->game->get_selected()->get_heading()));
	}
	
	this->atisview->repl["[METAR]"] = this->metar.to_string();
	//this->atisview->repl["[LEV]"] = Tools::tostr(this->game->get_level());
	this->atisview->repl["[DEPARTURE]"] = this->atis->get_departure_runway().get_name();
	this->atisview->repl["[LANDING]"] = this->atis->get_landing_runway().get_name();
	this->atisview->repl["[LEVEL]"] = Tools::tostr(this->atis->get_transition_level());
	this->atisview->repl["[ALTITUDE]"] = Tools::tostr(this->atis->get_transition_altitude());
	
	this->statview->repl["[SPE]"] = Tools::tostr(this->game->get_separation_errors());
	this->statview->repl["[RQD]"] = Tools::tostr(this->settings.required_handled);
}

void Gamecontroller::calculate_fps() {
	if (this->game_time > this->fps_end_time) {
		this->fps = this->frames / (fps_time / 1000.0);
		this->fps_end_time += this->fps_time;
		this->frames = 0;
	}
}

void Gamecontroller::update(double elapsed, Point& mouse) {
	//std::clog << "Gamecontroller::update(" << elapsed << ", " << mouse << ")" << std::endl;
	
	this->set_variables();
	
	if (this->state == ATIS) {
		if (this->atis->ok()) {
			this->state = GAME;
			this->gameview->load();
		} 
		
		this->menu = this->atis->get_menu();
		this->menuview->set_menu(this->menu);
		this->atisview->clear_screen();
		this->atisview->draw();
		this->atisview->draw_errors();
		this->menuview->draw();
		this->atisview->render();
	} else if (this->state == GAME) {
		this->game_time += elapsed;
		
		if (flash_message_begin + flash_message_time < game_time) {
			std::string tmp = this->game->get_message();
		
			if (tmp.length()) {
				this->set_flash_message(tmp);
			}
		}
		
		this->calculate_fps();
		this->game->update(elapsed);
		
		if (this->game->ok()) {
			this->state = STAT;
		}
		
		this->gameview->update_command(this->command);
		this->gameview->clear_screen();
		this->gameview->draw();
		this->gameview->draw_airfield(this->game->get_active_field());
		this->gameview->draw_planes(this->game->get_aircrafts(), this->game->get_selected(), mouse);
		
		if (this->game_time < (this->flash_message_begin + this->flash_message_time)) {
			this->gameview->flash_message(this->message);
		}
		
		this->gameview->render();
	} else if (this->state == STAT) {
		this->statview->clear_screen();
		this->statview->draw();
		this->statview->draw_points(this->game->get_points());
		this->statview->render();
	} else if (this->state == MENU) {
		this->menuview->clear_screen();
		this->menuview->draw();
		this->menuview->render();
	}
}

void Gamecontroller::set_flash_message(std::string message) {
	this->message = message;
	this->flash_message_begin = this->game_time;
}

void Gamecontroller::handle_mouse_click(Point& mouse) {
	this->metar.update("EFHK");
	
	if (this->state == GAME) {
		this->game->selected = NULL;

		std::list <Aircraft*> :: iterator plane;
		std::list <Aircraft*> aircrafts = this->game->get_aircrafts();

		for (plane = aircrafts.begin(); plane != aircrafts.end(); ++plane) {
			Point aircraft = this->gameview->calculate((*plane)->get_place());

			if (Tools::on_area(mouse, aircraft, 10)) {
				this->game->selected = (*plane);
				this->set_flash_message("Plane " + (*plane)->get_name() + " selected");
			}
		}
	} 	
}

void Gamecontroller::load_menu_items(std::string query, Menu*& menu) {
	std::clog << "Gamecontroller::load_menu_items(" << query << ", menu" << ")" << std::endl;
	menu->clear();
	std::list <std::string> arguments;
	arguments.push_back(this->command);
	
	Queryresult list = Database::get_result(Database::bind_param(query, arguments));
	
	for (unsigned int i = 0; i < list.size(); ++i) {
		std::string name = list(i, "ICAO");
		Menu_item t(i, name);
		
		menu->add_item(t);
	}
}

void Gamecontroller::handle_text_input() {
	//std::clog << "Gamecontroller::handle_text_input()" << std::endl;
    std::string t_command = this->command;
	
	if (this->state == GAME) {
		std::vector <std::string> tmp = Tools::split(";", this->command);
		
		if (tmp.size() > 1) {
			for (unsigned int i = 0; i < tmp.size(); ++i) {
				t_command = Tools::trim(tmp[i]);
				this->game->build_clearance(t_command);
				this->quicklist.push_back(t_command);
			}
		} else {
			this->game->build_clearance(command);
			this->quicklist.push_back(command);
		}
	} else if (this->state == MENU) {
		this->game->load(this->menu->get_selected().get_name());
		this->atis->set_airfield(this->game->get_active_field());
		this->metar.update(this->menu->get_selected().get_name());
		this->atis->load();
				
		this->state = ATIS;
		this->menuview->set_menu(menu);
	} else if (this->state == ATIS) {
		if (this->atis->ok() == true) {
			this->state = GAME;
		} else {
			this->atis->set_value(this->menu->get_selected().get_name());
		}
	}
}

void Gamecontroller::update_command(std::string command) {
	this->command = command;
}

bool Gamecontroller::is_ok() {
    return false;
}
