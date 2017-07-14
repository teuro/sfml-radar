#include "menu.hpp"

Menu::Menu(std::string id, std::string t_class) {
	this->id = id;
	this->t_class = t_class;
	this->selected = 0;
}

Menu::~Menu() {  }

void Menu::load() {
	std::clog << "Menu::load()" << std::endl;
	Menu_item t1(0, "EFHK", 38);
	Menu_item t2(1, "EFTP", 63);
	Menu_item t3(2, "EFTU", 89);
	
	this->items.push_back(t1);
	this->items.push_back(t2);
	this->items.push_back(t3);
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

void Menu::clear() {
	this->items.clear();
}