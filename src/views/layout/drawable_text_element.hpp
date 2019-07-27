#ifndef _DRAWABLE_TEXT_ELEMENT
#define _DRAWABLE_TEXT_ELEMENT

#include "drawable_element.hpp"

class Drawable_text_element {
private:
	std::string content;
public:
	explicit Drawable_text_element(std::string content);
	std::string get_content();
};

#endif