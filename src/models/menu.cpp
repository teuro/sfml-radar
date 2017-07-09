#include "menu.hpp"

Menu::Menu() { 
	this->level = 1;
	this->t_level = 0;
	
	this->airports.push_back("EFHK");
	this->airports.push_back("EFTU");
	this->airports.push_back("EFTP");
}

Menu::~Menu() { 

}

std::list <std::string> Menu::get_airports() {
	return this->airports;
}

void Menu::change_level(int change) {
	this->level += change;
}

int Menu::get_level() {
	return this->level;
}