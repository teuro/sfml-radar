#ifndef DRAWABLE_ELEMENT
#define DRAWABLE_ELEMENT

#include <string>
#include <map>
#include "../../models/point.hpp"
#include "../../tools/tools.hpp"
#include "../../tools/style/style.hpp"

struct Drawable_element {
private:
	std::string id;
	std::string s_class;
	std::string name;
protected:
	std::map <std::string, int> styles;
	
	Drawable_element(std::string t_name, std::string t_class = "", std::string t_id = "");
	~Drawable_element();
public:
	void set_style(Style& style);
	void set_style(std::string key, int value);
	int get_style(std::string key);
	
	std::string get_id();
	std::string get_class();
	std::string get_name();
};

#endif