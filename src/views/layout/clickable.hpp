#ifndef CLICKABLE_HPP
#define CLICKABLE_HPP

#include "drawable_element.hpp"
#include "../../tools/settings.hpp"
#include "../../tools/style/style.hpp"

#include <string>
#include <memory>

class Clickable : public Drawable_element{
private:
	int id;
	std::string content;
	std::shared_ptr <Settings> settings;
public:
	Clickable(std::string t_name, std::string t_class, std::string t_id, std::string cnt, std::shared_ptr <Settings> settings);
	~Clickable();
	void change_value(std::string name, int value);
	std::string get_name();
};

#endif
