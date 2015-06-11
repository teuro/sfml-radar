#include "atiscontroller.hpp"

Atiscontroller::Atiscontroller(Atisview& av, Settings& s, Atis& a, Game& g) : Controller(s), atisview(av), atis(a), game(g) { }

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

bool Atiscontroller::is_ok() {
	Metar metar = this->game.get_metar();
    int wind        = metar.get_wind_direction();
    int pressure    = metar.get_pressure();

    double dep_wind;
    double lnd_wind;

    std::vector <Runway> rwys = this->game.get_active_field()->get_runways();

    if (this->atis.get_departure_runway().length() > 0 && this->atis.get_landing_runway().length() > 0 && this->atis.get_transition_altitude() > 0 && this->atis.get_transition_level() > 0) {

        Runway* departure   = &(*std::find(rwys.begin(), rwys.end(), Tools::trim(this->atis.get_departure_runway())));
        Runway* landing     = &(*std::find(rwys.begin(), rwys.end(), Tools::trim(this->atis.get_landing_runway())));

        dep_wind = this->calculate_backwind(metar.get_wind_direction(), departure->get_heading());
        lnd_wind = this->calculate_backwind(metar.get_wind_direction(), landing->get_heading());

        int level = metar.get_correct_level(this->atis.get_transition_altitude());

        if (this->atis.get_transition_level() != level) {
            std::cerr << "Transition level is wrong " << level << std::endl;
        }

        if (dep_wind > 0) {
            std::cerr << "Departure runway has tailwind" << std::endl;
            return false;
        }

        if (lnd_wind > 0) {
            std::cerr << "Landing runway has tailwind" << std::endl;
            return false;
        }

        this->game.set_active_runways(departure, landing);
		
		for (unsigned int i = 0; i < 3; ++i) {
			this->game.create_plane();
    	}

        return true;
    }

    return false;
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