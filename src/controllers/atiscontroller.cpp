#include "atiscontroller.hpp"

Atiscontroller::Atiscontroller(Atisview& av, Settings& s, Atis& a) : Controller(&av, s), atis(a) { }

Atiscontroller::~Atiscontroller() { }

void Atiscontroller::handle_mouse_click(Point& mouse) {

}

void Atiscontroller::handle_mouse_release(Point& mouse_start, Point& mouse_end) {

}

void Atiscontroller::handle_mouse_wheel(int amount) {

}

void Atiscontroller::handle_function_keys(int key) {

}

void Atiscontroller::handle_text_input(std::string text) {

}
