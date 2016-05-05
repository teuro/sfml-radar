#include "menuitem.hpp"

Menuitem::Menuitem(std::string name) {
	this->name = name;
}

Menuitem::Menuitem() { }

Menuitem::~Menuitem() { }

std::string Menuitem::get_name() {
	return this->name;
}