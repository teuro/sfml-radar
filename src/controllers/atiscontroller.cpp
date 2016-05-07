#include "atiscontroller.hpp"

Atiscontroller::Atiscontroller(Atisview& av, Settings& s, Atis& a) : Controller(s), atisview(av), atis(a) { }

Atiscontroller::~Atiscontroller() { }

void Atiscontroller::handle_mouse_click(Point& mouse) {
	this->select_departure(mouse);
	this->select_landing(mouse);

    this->atis.set_transition_level(55);
    this->atis.set_transition_altitude(5000);
}

void Atiscontroller::select_departure(Point& mouse) {
	this->atis.set_departure_runway("22L");
}

void Atiscontroller::select_landing(Point& mouse) {
	this->atis.set_landing_runway("22L");
}

void Atiscontroller::handle_mouse_release(Point& mouse_start, Point& mouse_end) {

}

void Atiscontroller::handle_mouse_wheel(int amount) {

}

std::string Atiscontroller::handle_function_keys(int key) {
	return "";
}

void Atiscontroller::handle_text_input() {

}

void Atiscontroller::update(double elapsed, bool draw) {
    this->atis.update(elapsed);
	
	if (draw) {
		this->atisview.clear_screen();
		this->atisview.draw();
		this->atisview.draw_airfields(this->airfields);
		this->atisview.render();
	}
}

void Atiscontroller::update_command(std::string command) {
    this->atis.set_command("");
    this->atis.set_command(command);
}

void Atiscontroller::load() {
	std::clog << "Atiscontroller::load()" << std::endl;
	
	try {
		Queryresult fields = Database::get_result("SELECT ROWID, ICAO FROM airfields WHERE ICAO LIKE 'EF%K'");
		
		for (unsigned int i = 0; i < fields.size(); ++i) {
			this->airfields[Tools::tonumber<int>(fields(i, 0))] = fields(i, 1);
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