#include "playercontroller.hpp"

Playercontroller::Playercontroller(Playerview& pv, Settings& s, Player& p) : Controller(s), playerview(pv), player(p) { }

Playercontroller::~Playercontroller() { }

void Playercontroller::handle_mouse_click(Point& mouse) {

}

void Playercontroller::handle_mouse_release(Point& mouse_start, Point& mouse_end) {

}

void Playercontroller::handle_mouse_wheel(int amount) {

}

std::string Playercontroller::handle_function_keys(int key) {
	
	return "";
}

void Playercontroller::handle_text_input() {

}

void Playercontroller::update_command(std::string command) {

}

bool Playercontroller::is_ok() {
    return true;
}
