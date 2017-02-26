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
class Cell {
public:
	Cell(std::string content);
	
	std::string get_content();
	void set_content(std::string cnt);
private:
	std::string content;
};

/**
	* Row 
	* Contains list of cells
**/

class Row {
public:
	std::list <Cell> get_cells();
	void add_cell(Cell& cell);
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
	std::list <Row> get_rows();
	void add_row(Row& row);
	void add_cell(Cell& cell);
	int get_max_length();
private:
	std::list <Row> rows;
};

#endif