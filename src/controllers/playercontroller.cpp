#include "playercontroller.hpp"

Playercontroller::Playercontroller(Drawsurface& d, Settings& s) : Controller(s, d) { 
	this->playerview = new Playerview(this->drawer, this->settings);
	this->player = new Player(this->settings);
}

Playercontroller::~Playercontroller() { 
	delete this->playerview;
	delete this->player;
}

void Playercontroller::handle_mouse_click(Point& mouse) {

}

void Playercontroller::handle_mouse_release(Point& mouse_start, Point& mouse_end) {

}

void Playercontroller::handle_mouse_wheel(int amount) {

}

std::string Playercontroller::handle_function_keys(int key) {
	return this->command;
}

void Playercontroller::handle_text_input() {

}

void Playercontroller::update_command(std::string command) {

}

bool Playercontroller::is_ok() {
    return true;
}

void Playercontroller::load() {
	std::clog << "Playercontroller::load()" << std::endl;
}
