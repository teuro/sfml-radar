#include "atiscontroller.hpp"

Atiscontroller::Atiscontroller(Drawsurface& d, Settings& s, Atis& a) : Controller(s, d), atis(a) { 
	this->metar = new Metar;
	this->atisview = new Atisview(this->drawer, this->settings);
}

Atiscontroller::~Atiscontroller() { 
	delete this->metar;
	delete this->atisview;
}

void Atiscontroller::handle_mouse_click(Point& mouse) {
	this->select_departure(mouse);
	this->select_landing(mouse);
}

void Atiscontroller::select_departure(Point& mouse) {
	std::string tmp = this->atisview->get_dep_runway_name(mouse);
	
	if (tmp.length() > 0) {
		this->atis.set_departure_runway(tmp);
	}
}

void Atiscontroller::select_landing(Point& mouse) {
	std::string tmp = this->atisview->get_lnd_runway_name(mouse);
	
	if (tmp.length() > 0) {
		this->atis.set_landing_runway(tmp);
	}
}

void Atiscontroller::handle_mouse_release(Point& mouse_start, Point& mouse_end) {

}

void Atiscontroller::handle_mouse_wheel(int amount) {

}

std::string Atiscontroller::handle_function_keys(int key) {
	return this->command;
}

void Atiscontroller::handle_text_input() {
	std::vector <std::string> tmp = Tools::split(" ", this->command);
	
	if (tmp.size() == 2) {
		if (tmp[0] == "trlevel") {
			this->atis.set_transition_level(Tools::tonumber<int>(tmp[1]));
		} else if (tmp[0] == "tralt") {
			this->atis.set_transition_altitude(Tools::tonumber<int>(tmp[1]));
		}
	}
}

void Atiscontroller::update(double elapsed, bool draw) {
    this->atis.update(elapsed);
    this->metar->update("EFHK");
	
	if (draw) {
		this->atisview->clear_screen();
		this->atisview->add_element("Input", "input", "data", this->command);
		this->atisview->add_element("Metar", "metar", "data", this->metar->get_metar());
		this->atisview->draw();
		this->atisview->draw_runways(this->runways);
		this->atisview->render();
	}
}

void Atiscontroller::update_command(std::string input) {
	this->command = input;
}

void Atiscontroller::load() {
	std::clog << "Atiscontroller::load()" << std::endl;
	
	try {
		Queryresult rwys = Database::get_result("SELECT ROWID, name FROM runways WHERE airfield_id = 38");
		
		for (unsigned int i = 0; i < rwys.size(); ++i) {
			this->runways.push_back(rwys(i, 1));
		}
	} catch ( std::runtime_error& e ) {
		throw;
	} catch ( std::logic_error& e ) {
		throw;
	}
	
	std::clog << "Atiscontroller loaded" << std::endl;
}

bool Atiscontroller::is_ok() {
	if ((this->atis.get_departure_runway().length() > 0) && (this->atis.get_landing_runway().length() > 0) && (this->atis.get_transition_level() > 0) && (this->atis.get_transition_altitude() > 0)) {
		return true;
	}
	
	return false;
}

double Atiscontroller::calculate_backwind(double wind_direction_deg, double runway_heading) {
	double wind_direction_rad = Tools::deg2rad(wind_direction_deg);
	double t_wind = wind_direction_rad + Tools::get_PI();
	
	while (t_wind > (2 * Tools::get_PI())) {
		t_wind -= Tools::get_PI() * 2;
	}
	
	while (t_wind < 0) {
		t_wind += Tools::get_PI() * 2;
	}
	
	return std::cos(std::abs(t_wind - runway_heading));
}