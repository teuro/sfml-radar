#include "controller.hpp"

Controller::Controller(std::shared_ptr <Settings> s, Drawsurface& d) : settings(s), drawer(d) { 
	this->frames = 0;
	this->fps_time = 0;
	this->fps_end_time = 0;
	this->fps = 0;
}

Controller::~Controller() { }