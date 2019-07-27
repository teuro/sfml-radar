#ifndef DRAWABLE_TABLE
#define DRAWABLE_TABLE

#include "drawable_element.hpp"

/**
	* Drawable_table and helper classe Cell and Row
	* With this You may create tables more easier.
**/


/**
	* Cell 
	* Contains only pure string
**/
class Cell : public Drawable_element {
public:
	explicit Cell(std::string content, std::string t_name = "tr", std::string t_class = "", std::string t_id = "");
	explicit Cell(std::string content, std::string t_name, std::set <std::string> classes, std::string t_id);
	
	std::string get_content();
	void set_content(std::string cnt);
private:
	std::string content;
};

/**
	* Row 
	* Contains list of cells
**/

class Row : public Drawable_element {
public:
	Row(std::string t_name = "tr", std::string t_class = "", std::string t_id = "");
	Row(std::string t_name, std::set <std::string> classes, std::string t_id);
	std::list <Cell> get_cells();
	void add_cell(Cell& cell);
	void set_class(std::string s_class) override;
	void clear_classes() override;
private:
	std::list <Cell> cells;
};

/**
	* Drawable_table
	* Contains list of rows
**/

class Drawable_table : public Drawable_element {
public:
	Drawable_table(std::string t_name, std::string t_class, std::string t_id);
	Drawable_table(std::string t_name, std::set <std::string> classes, std::string t_id);
	std::list <Row> get_rows();
	void add_row(Row& row);
	std::string get_max_length() override;
	void set_class(std::string s_class) override;
	std::list <Row> :: iterator delete_row(std::list <Row> :: iterator rit);
	void clear_rows();
private:
	std::list <Row> rows;
};

#endif