#include "stat_view.hpp"
Statview::Statview(Drawsurface& d, std::shared_ptr <Settings> s, std::shared_ptr <Game> g) : View(d, s), game(g) { }

Statview::~Statview() { }

void Statview::draw(Point& ) { 
	this->clear_screen();

	View::draw();
	 
	this->draw_points();
}

void Statview::load() { 
	#ifdef DEBUG
	std::clog << "Statview::load()" << std::endl;
	#endif
	
	View::load("stat");
}

void Statview::draw_points() {
	std::map <std::string, Game_point> points = this->game->get_points();
	
	std::map <std::string, Game_point> :: iterator pit = points.begin();
	Drawable_table point_table("table", "", "points");
	
	int sum = 0;
	
	while (pit != points.end()) {
		Row row;
		Cell cell1(pit->first);
		row.add_cell(cell1);
		
		Cell cell2(Tools::tostr(pit->second.points, 4));
		row.add_cell(cell2);
		
		Cell cell3(Tools::totime(pit->second.in_time));
		row.add_cell(cell3);
		
		Cell cell4(Tools::totime(pit->second.out_time));
		row.add_cell(cell4);
		
		point_table.add_row(row);
		
		sum += pit->second.points;
		++pit;
	}
	
	Row row;
	
	Cell cell3("SUM");
	row.add_cell(cell3);
	
	Cell cell4(Tools::tostr(sum, 4));
	row.add_cell(cell4);
	
	point_table.add_row(row);
	
	this->style(point_table);
	this->draw_element(point_table);
}

void Statview::update() { }
std::string Statview::handle_click(Point&) { return ""; }