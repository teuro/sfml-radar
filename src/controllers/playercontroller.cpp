#include "playercontroller.hpp"

Playercontroller::Playercontroller(Playerview pv, Settings& s, Player& p) : Controller(&pv, s), player(p) { }

Playercontroller::~Playercontroller() { }

void Playercontroller::handle_mouse_click(Point& mouse) {

}

void Playercontroller::handle_mouse_release(Point& mouse_start, Point& mouse_end) {

}

void Playercontroller::handle_mouse_wheel(int amount) {

}

void Playercontroller::handle_function_keys(int key) {

}

void Playercontroller::handle_text_input(std::string text) {

}
