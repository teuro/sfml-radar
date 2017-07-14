#include "menu_item.hpp"

Menu_item::Menu_item(int id, std::string name, int value) {
	this->id = id;
	this->value = value;
	this->name = name;
}

Menu_item::~Menu_item() { }

int Menu_item::get_id() {
	return this->id;
}

int Menu_item::get_value() {
	return this->value;
}

std::string Menu_item::get_name() {
	return this->name;
}