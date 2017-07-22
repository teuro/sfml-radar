#include "menu.hpp"

Menu::Menu(std::string id, std::string t_class) {
	this->id = id;
	this->t_class = t_class;
	this->selected = 0;
}

Menu::~Menu() {  }

void Menu::load() {
	std::clog << "Menu::load()" << std::endl;
}

void Menu::change_selection(int change) {
	this->selected += change;
	
	if (this->selected >= this->items.size()) {
		this->selected = 0;
	} 
}

Menu_item Menu::get_selected() {
	return this->items[this->selected];
}

std::string Menu::get_id() {
	return this->id;
}

std::string Menu::get_class() {
	return this->t_class;
}

std::vector <Menu_item> Menu::get_items() {
	return this->items;
}

void Menu::add_item(Menu_item& mi) {
	this->items.push_back(mi);
}

void Menu::add_item(std::string name) {
	Menu_item mi(this->items.size(), name);
	this->items.push_back(mi);
}

void Menu::add_items(std::list <std::string> items) {
	std::list <std::string> :: iterator it = items.begin();
	
	while (it != items.end()) {
		this->add_item((*it));
		
		++it;
	}
}

void Menu::add_items(std::list <int> items) {
	std::list <int> :: iterator it = items.begin();
	
	while (it != items.end()) {
		this->add_item(Tools::tostr(*it));
		++it;
	}
}

void Menu::add_items(std::vector <std::string> items) {
	std::vector <std::string> :: iterator it = items.begin();
	
	while (it != items.end()) {
		this->add_item((*it));
		++it;
	}
}

void Menu::add_items(std::vector <int> items) {
	std::vector <int> :: iterator it = items.begin();
	
	while (it != items.end()) {
		this->add_item(Tools::tostr(*it));
		++it;
	}
}

void Menu::clear() {
	this->items.clear();
}