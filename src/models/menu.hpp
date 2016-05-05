#ifndef MENU_HPP
#define MENU_HPP

#include "menuitem.hpp"
#include "point.hpp"
#include <vector>
#include <stdexcept>

class Menu {
private:
	std::vector <Menuitem> menuitems;
public:
	Menu();
	~Menu();
	
	Menuitem get_item(std::string item);
	Menuitem get_item(int item);
	std::vector <Menuitem>& get_items();
	void Add_item(Menuitem& menuitem);
	void select(Point& m);
	Menuitem selected;
};

#endif