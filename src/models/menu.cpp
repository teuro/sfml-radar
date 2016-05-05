#include "menu.hpp"

Menu::Menu() { }
Menu::~Menu() { }

Menuitem Menu::get_item(std::string item) {
	for (unsigned int i = 0; i < this->menuitems.size(); ++i) {
		if (item == this->menuitems[i].get_name()) {
			return this->menuitems[i];
		}
	}
	
	throw std::runtime_error("Requested menuitem " + item + " not found");
	return this->menuitems[0];
}

std::vector <Menuitem>& Menu::get_items() {
	return this->menuitems;
}

void Menu::Add_item(Menuitem& menuitem) {
	this->menuitems.push_back(menuitem);
}

void Menu::select(Point& m) {
	selected = this->menuitems[0];
}

Menuitem Menu::get_item(int i) {
	return this->menuitems[i];
}