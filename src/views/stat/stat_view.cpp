#include "stat_view.hpp"
Statview::Statview(Drawsurface& d, std::shared_ptr <Settings> s, std::shared_ptr <Game> g) : View(d, s), game(g) { }

Statview::~Statview() { }

void Statview::draw() { 
	this->clear_screen();

	View::draw();
	 
	this->draw_points();
}

void Statview::load() { 
	View::load("stat");
}

void Statview::draw_points() {
	std::list <Game_point> points = this->game->get_points();
	
	std::list <Game_point> :: iterator pit = points.begin();
	Drawable_table point_table("table", "", "points");
	
	int sum = 0;
	
	while (pit != points.end()) {
		Row row;
		point_table.add_row(row);
		Cell cell1(pit->plane);
		point_table.add_cell(cell1);
		
		Cell cell2(Tools::tostr(pit->points, 4));
		point_table.add_cell(cell2);
		sum += pit->points;
		++pit;
	}
	
	Row row;
	point_table.add_row(row);
	Cell cell3("SUM");
	point_table.add_cell(cell3);
	
	Cell cell4(Tools::tostr(sum, 4));
	point_table.add_cell(cell4);
	
	this->style(point_table);
	this->draw_element(point_table);
}