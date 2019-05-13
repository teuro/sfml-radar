#ifndef DRAWABLE_HEADER
#define DRAWABLE_HEADER

#include "drawable_element.hpp"

class Header : public Drawable_element {
public:
	Header(std::string content, std::string t_name, std::string t_class, std::string t_id);
	Header(std::string content, std::string t_name, std::set <std::string> classes, std::string t_id);
	
	std::string get_content();
	void set_content(std::string cnt);
private:
	std::string content;
};

#endif