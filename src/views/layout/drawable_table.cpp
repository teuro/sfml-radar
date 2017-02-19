#include "drawable_table.hpp"

Cell::Cell(std::string cnt) : content(cnt) { }

std::string Cell::get_content() { 
	return this->content; 
}

void Cell::set_content(std::string cnt) { 
	this->content = cnt; 
}

void Row::add_cell(Cell& cell) { 
	#ifdef DEBUG
	std::clog << "Row::add_cell(" << cell.get_content() << ")" << std::endl;
	#endif
	this->cells.push_back(cell); 
}

Drawable_table::Drawable_table(std::string t_name, std::string t_class, std::string t_id) : Drawable_element(t_name, t_class, t_id) { }

std::list <Row> Drawable_table::get_rows() {
	return this->rows; 
}

void Drawable_table::add_row(Row& row) {
	#ifdef DEBUG
	std::clog << "Drawable_table::add_row()" << std::endl;
	#endif
	this->rows.push_back(row); 
}

void Drawable_table::add_cell(Cell& cell) {
	#ifdef DEBUG
	std::clog << "Drawable_table::add_cell(" << cell.get_content() << ")" << std::endl;
	#endif
	this->rows.back().add_cell(cell); 
}

std::list <Cell> Row::get_cells() { 
	return this->cells; 
}