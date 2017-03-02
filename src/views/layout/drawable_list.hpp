#ifndef DRAWABLE_LIST
#define DRAWABLE_LIST

#include "drawable_element.hpp"

class Drawable_list : public Drawable_element {
public:
	Drawable_list(std::string t_name, std::string t_class, std::string t_id);
	Drawable_list(std::string t_name, std::list <std::string> classes, std::string t_id);
	Drawable_list();
	void add_element(std::string content);
	std::list <std::string> get_elements();
	void clear_content();
private:
	std::list <std::string> elements;
};

#endif