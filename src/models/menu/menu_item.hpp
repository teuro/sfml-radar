#ifndef MENU_ITEM
#define MENU_ITEM

#include <iostream>

class Menu_item {
public:
	Menu_item(int id, std::string name, int value = 0);
	~Menu_item();
	
	int get_id();
	int get_value();
	std::string get_name();
private:
	int id;
	int value;
	std::string name;
};

#endif
