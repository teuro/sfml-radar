#ifndef DRAWABLE_CONTENT
#define DRAWABLE_CONTENT

#include "drawable_element.hpp"

class Drawable_list_item : public Drawable_element {
private:
	std::string content;
public:
	Drawable_list_item(std::string content, std::string t_class, std::string t_id);
	Drawable_list_item(std::string content, std::list <std::string> classes, std::string t_id);
	
	std::string get_content();
};

#endif