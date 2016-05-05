#include "atiscontroller.hpp"

Atiscontroller::Atiscontroller(Atisview& av, Settings& s, Atis& a) : Controller(s), atisview(av), atis(a) { }

Atiscontroller::~Atiscontroller() { }

void Atiscontroller::handle_mouse_click(Point& mouse) {

}

void Atiscontroller::handle_mouse_release(Point& mouse_start, Point& mouse_end) {

}

void Atiscontroller::handle_mouse_wheel(int amount) {

}

std::string Atiscontroller::handle_function_keys(int key) {
	
	return "";
}

void Atiscontroller::handle_text_input() {
    std::vector <std::string> parts = Tools::split(":", this->atis.get_command());
    std::string target = parts[0];

    if (target == "departure") {
        this->atis.set_departure_runway(parts[1]);
    } else if (target == "landing") {
        this->atis.set_landing_runway(parts[1]);
    } else if (target == "altitude") {
        this->atis.set_transition_altitude(Tools::tonumber<int>(parts[1]));
    } else if (target == "level") {
        this->atis.set_transition_level(Tools::tonumber<int>(parts[1]));
    }
}

void Atiscontroller::update(double elapsed, bool draw) {
    this->atis.update(elapsed);
    this->atisview.clear_screen();
    this->atisview.draw(this->atis.get_departure_runway(), this->atis.get_landing_runway(), this->atis.get_transition_altitude(), this->atis.get_transition_level());
    this->atisview.render();
}

void Atiscontroller::update_command(std::string command) {
    this->atis.set_command("");
    this->atis.set_command(command);
}

void Atiscontroller::load() {
	std::clog << "Atiscontroller::load()" << std::endl;
}

bool Atiscontroller::is_ok() {
    return true;
}

double Atiscontroller::calculate_backwind(double wind_direction_deg, double runway_heading) {
	//std::clog << "Wind direction = " << wind_direction_deg << " <=> " << Tools::rad2deg(runway_heading) << std::endl;
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