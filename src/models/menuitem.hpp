#ifndef MENUITEM_HPP
#define MENUITEM_HPP

#include <string>

class Menuitem {
private:
	std::string name;
public:
	Menuitem(std::string name);
	Menuitem();
	~Menuitem();
	std::string get_name();
};

#endif