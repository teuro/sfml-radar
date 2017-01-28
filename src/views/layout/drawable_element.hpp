#ifndef DRAWABLE_ELEMENT
#define DRAWABLE_ELEMENT

#include <string>
#include <list>
#include "../../models/point.hpp"
#include "../../tools/tools.hpp"
#include "../../tools/style/style.hpp"

struct Drawable_element {
private:
	std::string name;
	std::string s_class;
	std::string id;
	
	void calculate_styles();
protected:
	std::list <Style> styles;
	Style style;
	
	Drawable_element(std::string t_name, std::string t_class = "", std::string t_id = "");
	~Drawable_element();
public:
	void set_style(Style& style);

	Style& get_style();	
	
	std::string get_id();
	std::string get_class();
	std::string get_name();
};

#endif