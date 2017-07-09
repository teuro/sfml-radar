#ifndef MENU_HPP
#define MENU_HPP

#include <iostream>
#include <string>
#include <list>

class Menu {
public:
	Menu();
	~Menu();
	
	std::list <std::string> get_airports();
	
	void change_level(int change);
	void load();
	std::string get_airport();
	int get_level();
private:
	int level;
	int t_level;
	std::list <std::string> airports;
};

#endif