#include "gamecontroller.hpp"

Gamecontroller::Gamecontroller(Gameview& gv, Settings& s, Game& g) : Controller(&gv, s), game(g) {
    this->function_key = Tools::SPEED;
}

Gamecontroller::~Gamecontroller() { }

void Gamecontroller::handle_function_keys(int action) {
    this->function_key = action;
}

void Gamecontroller::handle_mouse_release(Point& mouse_start, Point& mouse_end) {
    double distance_px  = Tools::distancePX(mouse_end, mouse_start);
    double angle_rad    = Tools::angle(mouse_start, mouse_end);
    double distance_nm  = (distance_px * this->settings.zoom) / this->settings.screen_width;

    Coordinate center = Tools::calculate(this->game.get_centerpoint(), angle_rad, distance_nm, true, true);

    this->game.set_centerpoint(center);
    this->function_key = Tools::NONE;
}

void Gamecontroller::handle_mouse_wheel(int amount) {
    if (this->function_key > 0) {
        switch (this->function_key) {
            case Tools::SPEED:
                this->game.set_cl_spd(amount);
                break;
            case Tools::ALTITUDE:
                this->game.set_cl_alt(amount*100);
                break;
            case Tools::HEADING:
                this->game.set_cl_hdg(amount*5);
                break;
            default:
                break;
        }
    } else {
        this->settings.zoom += (amount * 10);

        if (this->settings.zoom < 10) {
            this->settings.zoom = 10;
        }
    }
}

void Gamecontroller::update(double elapsed) {
    this->game.update(elapsed);
}

void Gamecontroller::set_centerpoint(Coordinate& cp) {
    this->game.set_centerpoint(cp);
}

Coordinate& Gamecontroller::get_centerpoint() {
    return this->game.get_centerpoint();
}

void Gamecontroller::handle_mouse_click(Point& mouse) {
    this->game.select_aircraft(mouse);
}

void Gamecontroller::load() {
    std::clog << "Gamecontroller::load()" << std::endl;
    this->game.load();
}

void Gamecontroller::handle_text_input(std::string text) {
    Clearance cl(this->game.get_selected()->get_name(), this->game.get_duration(), this->game.get_cl_spd(), this->game.get_cl_alt(), this->game.get_cl_hdg(), 1);
    this->game.get_selected()->set_clearance(cl);
}
