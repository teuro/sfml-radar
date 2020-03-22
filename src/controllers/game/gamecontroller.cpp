#include "gamecontroller.hpp"

Gamecontroller::Gamecontroller(std::shared_ptr <Settings> s, Drawsurface& d) : Controller(s, d), out("developer.log", std::ios::out) { 
	#ifdef DEBUG
	out << "Gamecontroller::Gamecontroller()" << std::endl;
	#endif
	
	this->frames = 0;
	this->fps_time = 2000;
	this->fps_end_time = 4000;
	this->flash_message_begin = 0;
	this->flash_message_time = 2500;
	this->quick_iterator = this->quicklist.begin();
	
	this->level_selected = false;
	this->airport_selected = false;
	
	this->state = MENU;
	this->function_key = -1;
	this->game_time = 0;
}

Gamecontroller::~Gamecontroller() { }

void Gamecontroller::load() {
	#ifdef DEBUG
	out << "Gamecontroller::load()" << std::endl;
	#endif
	
	std::shared_ptr <Menu> m1(new Menu("airport", "airport"));
	
	this->airports 		= m1;
	
	this->load_menu_items("SELECT ICAO FROM airfields", this->airports);
	
	std::shared_ptr <Atis> a(new Atis(this->settings));
	this->atis = a;
	
	std::shared_ptr <Game> g(new Game(this->settings, this->atis));
	this->game = g;
	
	std::shared_ptr <Menuview> mv (new Menuview(this->drawer, this->settings, this->airports));	
	std::shared_ptr <Atisview> av (new Atisview(this->drawer, this->settings, this->atis));
	std::shared_ptr <Gameview> gv (new Gameview(this->drawer, this->settings, this->game));
	std::shared_ptr <Statview> sv (new Statview(this->drawer, this->settings, this->game));
	
	this->views[this->MENU] = mv;
	this->views[this->MENU]->load();
	
	this->views[this->ATIS] = av;
	this->views[this->ATIS]->load();
			
	this->views[this->GAME] = gv;
	this->views[this->GAME]->load();
		
	this->views[this->STAT] = sv;
}

void Gamecontroller::handle_function_keys(int key) {
	#ifdef DEBUG
	std::clog << "Gamecontroller::handle_function_keys(" << key << ")" << std::endl;
	#endif
	
	switch (key) {
		case 60:
			++this->quick_iterator;
			
			if (this->quick_iterator == this->quicklist.end()) {
				this->quick_iterator = this->quicklist.begin();
			} 
			
			this->command = (*this->quick_iterator);
			break;
	}
	
	this->handle_text_input();
}

void Gamecontroller::handle_mouse_release(Point& mouse_start, Point& mouse_end) {
	#ifdef DEBUG
	out << "Gamecontroller::handle_mouse_release(" << mouse_start << ", " <<  mouse_end << ")" << std::endl;
	#endif
	
	if (this->state == GAME) {
		double distance_px  = Tools::distancePX(mouse_start, mouse_end);
		double angle_rad    = Tools::fix_angle(Tools::angle(mouse_start, mouse_end) + Tools::get_PI() / 2.0);
		double distance_nm  = this->views[state]->distanceNM(distance_px);
		
		if (distance_nm > 0.5 && distance_nm < 100) {
			Coordinate cp = Tools::calculate(this->settings->get_centerpoint(), angle_rad, distance_nm);
			this->settings->set_centerpoint(cp);
		}
	}
}

void Gamecontroller::handle_mouse_wheel(int amount) {
	#ifdef DEBUG
	out << "Gamecontroller::handle_mouse_wheel(" << amount << ")" << std::endl;
	#endif
    if (this->state == GAME) {
		this->settings->zoom += (-amount * 1);
		
		if (this->settings->zoom < this->settings->zoom_min) {
			this->settings->zoom = this->settings->zoom_min;
		} else if (this->settings->zoom > this->settings->zoom_max) {
			this->settings->zoom = this->settings->zoom_max;
		}
	} else if (this->state == MENU) {
		this->airports->change_selection(-amount);
	} else if (this->state == ATIS) {
		this->atis->update(-amount);
	}
}

void Gamecontroller::set_variables() {
	#ifdef DEBUG
	out << "Gamecontroller::set_variables()" << std::endl;
	#endif
	
	if (this->state == MENU) {
		this->views[MENU]->repl["[MPL]"] = Tools::tostr(this->settings->max_planes);
		this->views[MENU]->repl["[RQD]"] = Tools::tostr(this->settings->required_handled);
		this->views[MENU]->repl["[LEV]"] = Tools::tostr(this->game->get_level());
	} else if (this->state == GAME) {
		this->views[GAME]->repl["[PLH]"] = Tools::tostr(this->game->get_handled_planes());
		this->views[GAME]->repl["[METAR]"] = this->metar->to_string();
		this->views[GAME]->repl["[TIME]"] = Tools::totime(this->game_time, "H:i:s");
		this->views[GAME]->repl["[PLC]"] = Tools::tostr(this->game->get_planes_count());
		this->views[GAME]->repl["[SPE]"] = Tools::tostr(this->game->get_separation_errors());	
		this->views[GAME]->repl["[PLN]"] = (this->game->get_new_plane() > this->game_time) ? Tools::totime(this->game->get_new_plane() - this->game_time, "i:s") : this->views[GAME]->repl["[PLN]"] = "no plane";
		this->views[GAME]->repl["[SPK]"] = Tools::tostr(this->settings->max_separation_errors);
		this->views[GAME]->repl["[RQD]"] = Tools::tostr(this->settings->required_handled);
		this->views[GAME]->repl["[DEP]"] = Tools::tostr(this->atis->get_departure_runway().get_name());
		this->views[GAME]->repl["[LND]"] = Tools::tostr(this->atis->get_landing_runway().get_name());
		this->views[GAME]->repl["[TRL]"] = Tools::tostr(this->atis->get_transition_level());
		this->views[GAME]->repl["[TRA]"] = Tools::tostr(this->atis->get_transition_altitude());
		this->views[GAME]->repl["[FPS]"] = Tools::tostr(this->fps);
		this->views[GAME]->repl["[PCNT]"] = Tools::tostr(this->game->get_game_points());
		this->views[GAME]->repl["[CLRE]"] = Tools::tostr(this->game->get_clearance_error());
		this->views[GAME]->repl["[CLRC]"] = Tools::tostr(this->game->get_clearance_count());
		this->views[GAME]->repl["[GRE]"] = Tools::tostr(this->game->get_game_error());
		this->views[GAME]->repl["[QNH]"] = Tools::tostr(this->metar->get_pressure());
		this->views[GAME]->repl["[WNDD]"] = Tools::tostr(this->metar->get_wind_direction());
		this->views[GAME]->repl["[WNDS]"] = Tools::tostr(this->metar->get_wind_speed());
		this->views[GAME]->repl["[HLDA]"] = Tools::tostr(this->game->get_holdings().size());
		this->views[GAME]->repl["[MXA]"] = Tools::tostr(Tools::round_nearest(Tools::rad2deg(this->atis->get_landing_runway().get_heading()) + this->settings->approach_angle, 10));
		this->views[GAME]->repl["[MNA]"] = Tools::tostr(Tools::round_nearest(Tools::rad2deg(this->atis->get_landing_runway().get_heading()) - this->settings->approach_angle, 10));
	} else if (this->state == ATIS) {
		this->views[ATIS]->repl["[METAR]"] = this->metar->to_string();
		this->views[ATIS]->repl["[LEV]"] = Tools::tostr(this->game->get_level());
		this->views[ATIS]->repl["[DEPARTURE]"] = this->atis->get_departure_runway().get_name();
		this->views[ATIS]->repl["[LANDING]"] = this->atis->get_landing_runway().get_name();
		this->views[ATIS]->repl["[LEVEL]"] = Tools::tostr(this->atis->get_transition_level());
		this->views[ATIS]->repl["[ALTITUDE]"] = Tools::tostr(this->atis->get_transition_altitude());
		this->views[ATIS]->repl["[RQD]"] = Tools::tostr(this->settings->required_handled);
	} else if (this->state == STAT) {
		this->views[STAT]->repl["[SPE]"] = Tools::tostr(this->game->get_separation_errors());
		this->views[STAT]->repl["[RQD]"] = Tools::tostr(this->settings->required_handled);
	}
}

void Gamecontroller::calculate_fps() {
	#ifdef DEBUG
	std::clog << "Gamecontroller::calculate_fps()" << std::endl;
	#endif
	
	if (this->game_time > this->fps_end_time) {
		this->fps = this->frames / (fps_time / 1000.0);
		this->fps_end_time += this->fps_time;
		this->frames = 0;
	}
}

void Gamecontroller::update(double elapsed, Point& mouse) {
	#ifdef DEBUG
	std::clog << "Gamecontroller::update(" << elapsed << ", " << mouse << ")" << std::endl;
	#endif
	
	if (this->state == MENU && this->airport_selected) {
		this->state = ATIS;
		
		this->game->load(this->airport_name);
		this->atis->set_airfield(this->game->get_active_field());
		std::shared_ptr <Metar> m(new Metar(settings, this->airport_name, 205));
		
		this->metar = m;
		this->metar->update();
		this->atis->load();
		this->atis->set_metar(this->metar);
		
		atis_items = this->atis->get_menu();
		
		this->views[ATIS]->set_menu(atis_items);
	} else if (this->state == ATIS && this->atis->ok()) {
		this->state = GAME;
		int max_amount_planes = (this->settings->required_handled < this->game->get_active_field()->get_inpoints().size()) ? this->settings->required_handled : this->game->get_active_field()->get_inpoints().size();
		
		this->game->create_planes(Tools::linear_random(1, max_amount_planes));
	} else if (this->state == GAME && this->game->ok()) {
		this->state = STAT;
		this->views[this->STAT]->load();
	}
	
	this->game_time += elapsed;
	
	this->set_variables();
	
	if (this->state == GAME) {
		std::string tmp = this->game->get_message();
		//this->metar->update_pressure();

		if (tmp.length()) {
			this->set_flash_message(tmp);
		}
		
		++this->frames;
		this->calculate_fps();
		this->game->update(elapsed);
	}
	
	this->views[this->state]->clear_screen();
	this->views[this->state]->update();
	this->views[this->state]->draw(mouse);
	
	if (this->game_time > this->flash_message_begin && this->game_time < (this->flash_message_begin + this->flash_message_time) && this->message.length()) {
		this->views[this->state]->flash_message(this->message);
	} 	
	
	this->views[this->state]->render();
}

void Gamecontroller::set_flash_message(std::string message) {
	this->message = message;
	
	this->flash_message_begin = this->game_time;
}

void Gamecontroller::handle_mouse_click(Point& mouse) {
	#ifdef DEBUG
	out << "Gamecontroller::handle_mouse_click()" << std::endl;
	#endif
	
	this->set_flash_message(this->views[this->state]->handle_click(mouse));
}

void Gamecontroller::load_menu_items(std::string query, std::shared_ptr <Menu> menu) {
	#ifdef DEBUG
	out << "Gamecontroller::load_menu_items(" << query << ", menu" << ")" << std::endl;
	#endif
	
	menu->clear();
	std::list <std::string> arguments;
	arguments.push_back(this->command);
	
	Database db;
	
	Queryresult list = db.get_result(db.bind_param(query, arguments));
	
	for (unsigned int i = 0; i < list.size(); ++i) {
		std::string name = list(i, "ICAO");
		Menu_item t(i, name);
		
		menu->add_item(t);
	}
}

void Gamecontroller::handle_text_input() {
	#ifdef DEBUG
	out << "Gamecontroller::handle_text_input()" << std::endl;
	#endif
	
    std::string t_command;
	
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
		this->airport_name = this->airports->get_selected().get_name();
		this->airport_selected = true;
	} else if (this->state == ATIS) {
		#ifdef DEBUG
		out << "Gamecontroller::handle_text_input(" << this->atis_items->get_selected().get_name() << ")" << std::endl;
		#endif
		
		this->atis->set_value(this->atis_items->get_selected().get_name());
	}
}

void Gamecontroller::update_command(std::string cmd) {
	#ifdef DEBUG
	out << "Gamecontroller::update_command(" << cmd << ")" << std::endl;
	#endif
	
	this->command = cmd;
	
	this->views[state]->update_command(this->command);
}
