#ifndef DRAWABLE_LIST
#define DRAWABLE_LIST

#include "drawable_element.hpp"
#include "drawable_list_item.hpp"

class Drawable_list : public Drawable_element {
public:
	Drawable_list(std::string t_name, std::string t_class, std::string t_id);
	Drawable_list(std::string t_name, std::set <std::string> classes, std::string t_id);
	Drawable_list();
	void add_element(std::string content, std::string t_class = "", std::string t_id = "");
	void add_element(std::string content, std::set <std::string> classes, std::string t_id);
	std::list <Drawable_list_item> get_elements();
	void clear_content();
	virtual std::string get_max_length();
	void set_class(std::string t_class);
private:
	std::list <Drawable_list_item> elements;
};

#endif