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
	
	std::vector <Drawable_table> :: iterator tit = tables.begin();
	
	while (tit != tables.end()) {
		if ((*tit).get_id() == std::string("points")) {
			int area = 0;
			int clearances = 0;
			
			std::list <Row> t_list = tit->get_rows();		
			std::list <Row> :: iterator rit = t_list.begin();
			
			Row replaced;
			Row sum_row;
			
			while (rit != t_list.end()) {
				if (rit->get_id() == "replace") {
					replaced = *rit;
				} 
				
				if (rit->get_id() == "sum") {
					sum_row = *rit;
				}
				
				++rit;
			}
			
			tit->clear_rows();
			
			std::list <aircraft> handled_planes = this->game->get_handled_planes_list();
			std::list <aircraft> :: iterator hpit = handled_planes.begin();
			double area_time;
			
			while (hpit != handled_planes.end()) {
				Aircraft& plane = *(*hpit);
				std::list <Cell> t_cells = replaced.get_cells();
				area_time = plane.get_out_time() - plane.get_in_time();
				
				area += area_time;
				clearances += plane.get_clearances();
				
				this->repl["[callsign]"] = plane.get_name();
				this->repl["[in_time]"] = Tools::totime(plane.get_in_time());
				this->repl["[out_time]"] = Tools::totime(plane.get_out_time());
				this->repl["[area_time]"] = Tools::totime(area_time);
				this->repl["[points]"] = Tools::tostr(plane.get_points(), 6);
				this->repl["[clearances]"] = Tools::tostr(plane.get_clearances(), 3);
				
				Row row;
				
				std::list <Cell> c_list = replaced.get_cells();
				
				std::list <Cell> :: iterator cit = c_list.begin();
				
				while (cit != c_list.end()) {
					Cell tmp_cell(Tools::replace(cit->get_content(), this->repl));

					row.add_cell(tmp_cell);
					++cit;
				}
				
				tit->add_row(row);
				
				++hpit;
			}
			
			this->repl["[AREA]"] = Tools::totime(area / this->game->get_handled_planes_list().size());
			this->repl["[SUM]"] = Tools::tostr(this->game->get_game_points(), 6);
			this->repl["[CLRC]"] = Tools::tostr(clearances, 3);
			
			Row row;
				
			std::list <Cell> c_list = sum_row.get_cells();
			std::list <Cell> :: iterator cit = c_list.begin();
			
			while (cit != c_list.end()) {
				Cell tmp_cell(Tools::replace(cit->get_content(), this->repl));

				row.add_cell(tmp_cell);
				++cit;
			}
			
			tit->add_row(row);
		}
		
		++tit;
	}
}

void Statview::draw_points() {
	
}

void Statview::update() { }

std::string Statview::handle_click(Point&) { return ""; }
