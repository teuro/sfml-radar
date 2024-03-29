#ifndef DRAWABLE_ELEMENT
#define DRAWABLE_ELEMENT

#include <string>
#include <list>
#include <set>
#include "../../models/coordinate/point.hpp"
#include <tools/tools.hpp>
#include "../../tools/style/style.hpp"

/**
	* Drawable_element
	* Base class for all drawable elements. This provides style, so You don't need to place nor take care of colors.
	* You should always use proper css-file. 
	* You may use either single class attribute or space separated words. There are no maximum amount of classes.
**/

struct Drawable_element {
protected:
	std::string name;
	std::string s_class;
	std::set <std::string> classes;
	std::string id;
	
	/**
		* calculate_styles
		* this element may contain many styles, so they need to compute as one. 
		* @param void
		* @return void
	**/
	void calculate_styles();

	std::list <Style> styles;
	Style style;
	
	explicit Drawable_element(std::string t_name, std::string t_class = "", std::string t_id = "");
	explicit Drawable_element(std::string t_name, std::set <std::string> classes, std::string t_id = ""); 
	Drawable_element();
	~Drawable_element();
public:
	/**
		* set_style
		* You may set proper style
		* @param Style
		* @return void
	**/
	
	void set_style(Style& style);
	
	/**
		* get_style
		* This is always computed style.
		* @param void
		* @return Style
	**/
	
	Style& get_style();	
	
	/**
		* get_id
		* @param void
		* @return std::string
	**/
	
	std::string get_id();
	
	/**
		* get_class
		* @param void
		* @return std::string
	**/
	
	std::string get_class();
	
	/**
		* get_classes
		* @param void
		* @return std::list <std::string>
	**/
	
	std::set <std::string> get_classes();
	
	/**
		* get_name
		* @param void
		* @return std::string
	**/
	
	std::string get_name();
	
	/**
		* set_class
		* @param std::string
		* @return void
	**/
	
	virtual void set_class(std::string t_class);
	
	/**
		* get maximum length
	**/
	
	virtual std::string get_max_length();
	
	/**
		* clear_classes
		* remove all classes
		* @param void
		/ @return void
	**/
	
	virtual void clear_classes();
};

#endif