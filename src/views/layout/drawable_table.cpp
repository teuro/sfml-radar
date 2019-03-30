#include "drawable_table.hpp"

Cell::Cell(std::string _content, std::string t_name, std::string t_class, std::string t_id) : Drawable_element(t_name, t_class, t_id), content(_content) { }
Cell::Cell(std::string _content, std::string t_name, std::list <std::string> t_classes, std::string t_id) : Drawable_element(t_name, t_classes, t_id), content(_content) { }

std::string Cell::get_content() { 
	return this->content; 
}

void Cell::set_content(std::string cnt) { 
	this->content = cnt; 
}

Row::Row(std::string t_name, std::string t_class, std::string t_id) : Drawable_element(t_name, t_class, t_id) { }
Row::Row(std::string t_name, std::list <std::string> classes, std::string t_id) : Drawable_element(t_name, classes, t_id) { }

void Row::add_cell(Cell& cell) { 
	#ifdef DEBUG
	std::clog << "Row::add_cell(" << cell.get_content() << ")" << std::endl;
	#endif
	this->cells.push_back(cell); 
}

Drawable_table::Drawable_table(std::string t_name, std::string t_class, std::string t_id) : Drawable_element(t_name, t_class, t_id) { }
Drawable_table::Drawable_table(std::string t_name, std::list <std::string> classes, std::string t_id) : Drawable_element(t_name, classes, t_id) { }

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

std::string Drawable_table::get_max_length() {
	std::list <Row> :: iterator rit = this->rows.begin();
	std::string max;
	
	while (rit != this->rows.end()) {
		std::list <Cell> c_list = rit->get_cells();
		std::list <Cell> :: iterator cit = c_list.begin();
		while (cit != c_list.end()) {
			if ((*cit).get_content().length() > max.length()) {
				max = (*cit).get_content();
			}
			++cit;
		}
		++rit;
	}
	
	return max;
}

std::list <Cell> Row::get_cells() { 
	return this->cells; 
}