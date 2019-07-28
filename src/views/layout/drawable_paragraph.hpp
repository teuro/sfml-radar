#ifndef DRAWABLE_PARAGRAPH
#define DRAWABLE_PARAGRAPH

#include "drawable_element.hpp"

class Paragraph : public Drawable_element {
public:
	Paragraph(std::string content, std::string t_name, std::string t_class, std::string t_id);
	Paragraph(std::string content, std::string t_name, std::set <std::string> classes, std::string t_id);
	
	std::string get_content();
private:
	std::string content;
};

#endif