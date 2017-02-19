#ifndef DRAWABLE_PARAGRAPH
#define DRAWABLE_PARAGRAPH

#include "drawable_element.hpp"

class Paragraph : public Drawable_element {
public:
	Paragraph(std::string content, std::string t_name, std::string t_class, std::string t_id);
	
	std::string get_content();
	void set_content(std::string cnt);
private:
	std::string content;
};

#endif