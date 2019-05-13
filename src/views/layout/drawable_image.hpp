#ifndef DRAWABLE_IMAGE
#define DRAWABLE_IMAGE

#include "drawable_element.hpp"

class Image : public Drawable_element {
public:
	Image(std::string source, std::string t_name, std::string t_class, std::string t_id);
	Image(std::string source, std::string t_name, std::set <std::string> classes, std::string t_id);
	
	std::string get_source();
	void set_source(std::string src);
private:
	std::string source;
};

#endif