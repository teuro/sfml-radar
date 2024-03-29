#include "drawable_paragraph.hpp"

Paragraph::Paragraph(std::string cnt, std::string t_name, std::string t_class, std::string t_id) : Drawable_element(t_name, t_class, t_id), content(cnt) { }
Paragraph::Paragraph(std::string cnt, std::string t_name, std::set <std::string> classes, std::string t_id) : Drawable_element(t_name, classes, t_id), content(cnt) { }

std::string Paragraph::get_content() {
	return this->content;
}
