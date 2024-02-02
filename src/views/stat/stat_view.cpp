#include "stat_view.hpp"

Statview::Statview(Drawsurface& d, std::shared_ptr <Settings> s, std::shared_ptr <Game> g) : View(d, s), game(g) { }

Statview::~Statview() { }

void Statview::draw(Point& mouse) {
	this->clear_screen();
	
	if (!this->loaded) {
		throw std::logic_error("Statview is not loaded");
	}
	
	this->View::draw();
}

void Statview::load() { 
	this->loaded = false;
	#ifdef DEBUG
	std::clog << "Statview::load()" << std::endl;
	#endif
	
	View::load("stat");
	
	std::vector <Drawable_table> :: iterator tit = tables.begin();
	int sum_points = 0;
	
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
			/** 
				* @todo
				* get data from controller
				* SELECT COUNT(clearances.clearance) AS clearances, planes.callsign, planes.game_id FROM planes LEFT JOIN clearances ON planes.rowid = clearances.plane_id WHERE planes.game_id = game_id GROUP BY planes.rowid ORDER BY clearances DESC, callsign ASC;
			**/
			Database db;
			//Queryresult handled_planes = this->game->get_stat_data();
			Queryresult handled_planes = db.get_result("SELECT COUNT(clearances.clearance) AS clearances, planes.callsign, planes.game_id, planes.time_in AS in_time, planes.time_out AS out_time FROM planes LEFT JOIN clearances ON planes.rowid = clearances.plane_id WHERE planes.game_id = game_id GROUP BY planes.rowid ORDER BY clearances DESC, callsign ASC");
			int area_time = 0;
			
			for (unsigned int i = 0; i < handled_planes.size(); ++i) {
				std::list <Cell> t_cells = replaced.get_cells();
				
				int in_time = Tools::toint(handled_planes(i, "in_time"));
				int out_time = Tools::toint(handled_planes(i, "out_time"));
				
				area_time = out_time - in_time;				
				
				area += area_time;
				clearances += Tools::toint(handled_planes(i, "clearances"));
				
				this->repl["[callsign]"] = handled_planes(i, "callsign");
				this->repl["[in_time]"] = Tools::totime(in_time);
				this->repl["[out_time]"] = Tools::totime(out_time);
				this->repl["[area_time]"] = Tools::totime(area_time);
				//this->repl["[points]"] = Tools::tostr(handled_planes(i, "points"), 8);
				this->repl["[clearances]"] = Tools::tostr(handled_planes(i, "clearances"), 3);
				
				//sum_points += Tools::toint(handled_planes(i, "points"));
				
				Row row;
				
				std::list <Cell> c_list = replaced.get_cells();
				
				std::list <Cell> :: iterator cit = c_list.begin();
				
				while (cit != c_list.end()) {
					Cell tmp_cell(Tools::replace(cit->get_content(), this->repl));

					row.add_cell(tmp_cell);
					++cit;
				}
				
				tit->add_row(row);
			}
			
			this->repl["[AREA]"] = Tools::totime(area / handled_planes.size());
			//this->repl["[SUM]"] = Tools::tostr(sum_points, 8);
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
	
	this->loaded = true;
}

void Statview::draw_points() {
	
}

void Statview::update() { }

std::string Statview::handle_click(Point&) { return ""; }
