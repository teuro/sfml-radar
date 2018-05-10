#include "gamecontroller.hpp"

Gamecontroller::Gamecontroller(std::shared_ptr <Settings> s, Drawsurface& d) : Controller(s, d) { 
	Metar metar;
	
	std::shared_ptr <Atis> a(new Atis(this->settings, this->metar));
	this->atis = a;
	
	std::shared_ptr <Game> g(new Game(this->settings, this->atis));
	this->game = g;
	
	this->settings->zoom = 110;
	this->frames = 0;
	this->fps_time = 5000;
	this->fps_end_time = 8500;
	this->flash_message_begin = -1;
	this->flash_message_time = 2500;
	quick_iterator = this->quicklist.begin();
	
	this->state = MENU;
}

Gamecontroller::~Gamecontroller() { }

void Gamecontroller::load() {
	std::clog << "Gamecontroller::load()" << std::endl;
	
	std::shared_ptr <Menu> m(new Menu("airport", "airport"));
	this->menu 		= m;
	
	this->load_menu_items("SELECT ICAO FROM airfields", this->menu);
	
	std::shared_ptr <Gameview> gv (new Gameview(this->drawer, this->settings, this->game));
	std::shared_ptr <Atisview> av (new Atisview(this->drawer, this->settings, this->atis));
	std::shared_ptr <Statview> sv (new Statview(this->drawer, this->settings, this->game));
	std::shared_ptr <Menuview> mv (new Menuview(this->drawer, this->settings, this->menu));
	
	this->views[this->MENU] = mv;
	this->views[this->ATIS] = av;
	this->views[this->GAME] = gv;
	this->views[this->STAT] = sv;
	
	this->views[this->MENU]->load();
	this->views[this->ATIS]->load();
	this->views[this->STAT]->load();
}

std::string Gamecontroller::handle_function_keys() {
	return this->command;
}

void Gamecontroller::handle_mouse_release(Point& mouse_start, Point& mouse_end) {
	//std::clog << "Gamecontroller::handle_mouse_release(" << mouse_start << ", " <<  mouse_end << ")" << std::endl;
    double distance_px  = Tools::distancePX(mouse_start, mouse_end);
    double angle_rad    = Tools::fix_angle(Tools::angle(mouse_start, mouse_end) + Tools::get_PI() / 2.0);
    double distance_nm  = this->views[GAME]->distanceNM(distance_px);
	
	if (distance_nm > 0.5 && distance_nm < 100) {
		Coordinate cp = Tools::calculate(this->settings->get_centerpoint(), angle_rad, distance_nm);
		this->settings->set_centerpoint(cp);
	}
}

void Gamecontroller::handle_mouse_wheel(int amount) {
    if (this->state == GAME) {
		this->settings->zoom += (-amount * 1);
		
		if (this->settings->zoom < this->settings->zoom_min) {
			this->settings->zoom = this->settings->zoom_min;
		} else if (this->settings->zoom > this->settings->zoom_max) {
			this->settings->zoom = this->settings->zoom_max;
		}
	} else if (this->state == MENU) {
		this->menu->change_selection(-amount);
	} else if (this->state == ATIS) {
		this->atis->update(-amount);
	}
}

void Gamecontroller::set_variables() {
	#ifdef DEBUG
	std::clog << "Gamecontroller::set_variables()" << std::endl;
	#endif
	this->views[MENU]->repl["[MPL]"] = Tools::tostr(this->settings->max_planes);
	this->views[MENU]->repl["[RQD]"] = Tools::tostr(this->settings->required_handled);
	this->views[MENU]->repl["[LEV]"] = Tools::tostr(this->game->get_level());
	
	this->views[GAME]->repl["[PLH]"] = Tools::tostr(this->game->get_handled_planes());
	this->views[GAME]->repl["[METAR]"] = this->metar.to_string();
	this->views[GAME]->repl["[TIME]"] = Tools::totime(this->game_time, "H:i:s");
	this->views[GAME]->repl["[PLC]"] = Tools::tostr(this->game->get_planes_count());
	this->views[GAME]->repl["[SPE]"] = Tools::tostr(this->game->get_separation_errors());
	if (this->game->get_new_plane() > this->game_time) {
		this->views[GAME]->repl["[PLN]"] = Tools::totime(this->game->get_new_plane() - this->game_time, "i:s");
	} else {
		this->views[GAME]->repl["[PLN]"] = "no plane";
	}
	this->views[GAME]->repl["[SPK]"] = Tools::tostr(this->settings->max_separation_errors);
	this->views[GAME]->repl["[RQD]"] = Tools::tostr(this->settings->required_handled);
	this->views[GAME]->repl["[DEP]"] = Tools::tostr(this->atis->get_departure_runway().get_name());
	this->views[GAME]->repl["[LND]"] = Tools::tostr(this->atis->get_landing_runway().get_name());
	this->views[GAME]->repl["[TRL]"] = Tools::tostr(this->atis->get_transition_level());
	this->views[GAME]->repl["[TRA]"] = Tools::tostr(this->atis->get_transition_altitude());
	this->views[GAME]->repl["[FPS]"] = Tools::tostr(this->fps);
	this->views[GAME]->repl["[CLRC]"] = Tools::tostr(this->game->get_clearances().size());
	this->views[GAME]->repl["[PCNT]"] = Tools::tostr(this->game->get_game_points());
	this->views[GAME]->repl["[CLRE]"] = Tools::tostr(this->game->get_clearance_error());
	this->views[GAME]->repl["[GRE]"] = Tools::tostr(this->game->get_game_error());
	this->views[GAME]->repl["[MXA]"] = Tools::tostr(Tools::round_nearest(Tools::rad2deg(this->atis->get_landing_runway().get_heading()) + this->settings->approach_angle, 10));
	this->views[GAME]->repl["[MNA]"] = Tools::tostr(Tools::round_nearest(Tools::rad2deg(this->atis->get_landing_runway().get_heading()) - this->settings->approach_angle, 10));
	
	this->views[ATIS]->repl["[METAR]"] = this->metar.to_string();
	//this->views[ATIS]->repl["[LEV]"] = Tools::tostr(this->game->get_level());
	this->views[ATIS]->repl["[DEPARTURE]"] = this->atis->get_departure_runway().get_name();
	this->views[ATIS]->repl["[LANDING]"] = this->atis->get_landing_runway().get_name();
	this->views[ATIS]->repl["[LEVEL]"] = Tools::tostr(this->atis->get_transition_level());
	this->views[ATIS]->repl["[ALTITUDE]"] = Tools::tostr(this->atis->get_transition_altitude());
	
	this->views[STAT]->repl["[SPE]"] = Tools::tostr(this->game->get_separation_errors());
	this->views[STAT]->repl["[RQD]"] = Tools::tostr(this->settings->required_handled);
}

void Gamecontroller::calculate_fps() {
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
	
	if (this->atis->ok() && this->state == ATIS) {
		this->state = GAME;
		this->views[GAME]->load();
		this->game->create_planes(Tools::rnd(3, this->settings->required_handled));
	} else if (this->game->ok()) {
		this->state = STAT;
	}
	
	this->game_time += elapsed;
	
	this->set_variables();
	
	this->views[this->state]->clear_screen();
	this->views[this->state]->update();
	this->views[this->state]->draw(mouse);
	
	if (this->game_time < (this->flash_message_begin + this->flash_message_time)) {
		this->views[this->state]->flash_message(this->message);
	}
	
	if (this->state == GAME) {
		std::string tmp = this->game->get_message();

		if (tmp.length()) {
			this->set_flash_message(tmp);
		}
		
		this->calculate_fps();
		this->game->update(elapsed);
	}
	
	this->views[this->state]->render();
}

void Gamecontroller::set_flash_message(std::string message) {
	this->message = message;
	this->flash_message_begin = this->game_time;
}

void Gamecontroller::handle_mouse_click(Point& mouse) {
	this->message = this->views[this->state]->handle_click(mouse);
}

void Gamecontroller::load_menu_items(std::string query, std::shared_ptr <Menu> menu) {
	std::clog << "Gamecontroller::load_menu_items(" << query << ", menu" << ")" << std::endl;
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
		menu = this->atis->get_menu();
		this->views[MENU]->set_menu(menu);
	} else if (this->state == ATIS) {
		if (this->atis->ok() == true) {
			this->state = GAME;
		} else {
			std::clog << this->menu->get_selected().get_name() << std::endl;
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

void Gamecontroller::update_centerpoint() {
	std::clog << "Gamecontroller::update_centerpoint() " <<  this->game->get_active_field()->get_place() << std::endl;
	//this->settings->set_centerpoint(this->game->get_active_field()->get_place());
}
