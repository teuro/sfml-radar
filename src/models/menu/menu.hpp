#ifndef MENU_HPP
#define MENU_HPP

#include <iostream>
#include <string>
#include <vector>

#include "menu_item.hpp"

#include "../../tools/database/database.hpp"
#include "../../tools/text_tools.hpp"

class Menu {
public:
	Menu(std::string id, std::string t_class);
	~Menu();
	
	void change_selection(int change);
	Menu_item get_selected();
	void load();
	
	std::string get_id();
	std::string get_class();
	
	std::vector <Menu_item> get_items();
	void add_item(Menu_item& mi);
	void add_item(std::string name);
	void add_items(std::list <int> items);
	void add_items(std::list <std::string> items);
	void add_items(std::vector <int> items);
	void add_items(std::vector <std::string> items);
	void clear();
private:
	std::string id;
	std::string t_class;
	unsigned int selected;
	std::vector <Menu_item> items;
};

#endif