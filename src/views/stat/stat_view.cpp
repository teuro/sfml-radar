#include "stat_view.hpp"
Statview::Statview(Drawsurface& d, Settings& s) : View(d, s) { }

Statview::~Statview() { }

void Statview::draw() { 
	 View::draw();
}

void Statview::load() { 
	std::clog << "Statview::load()" << std::endl;
	View::load("stat");
}

void Statview::draw_points(std::list <Game_point> points) {
	std::list <Game_point> :: iterator pit = points.begin();
	Drawable_table point_table("table", "", "points");
	
	int sum = 0;
	
	while (pit != points.end()) {
		Row row;
		point_table.add_row(row);
		Cell cell1(pit->plane);
		point_table.add_cell(cell1);
		
		Cell cell2(Tools::tostr(pit->points, 3));
		point_table.add_cell(cell2);
		sum += pit->points;
		++pit;
	}
	
	Row row;
	point_table.add_row(row);
	Cell cell3("SUM");
	point_table.add_cell(cell3);
	
	Cell cell4(Tools::tostr(sum, 3));
	point_table.add_cell(cell4);
	
	this->style(point_table);
	this->draw_element(point_table);
}