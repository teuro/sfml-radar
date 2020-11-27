#include "view.hpp"

void View::calculate_coordinate_limits() {
	Coordinate c(Tools::calculate(this->settings->get_centerpoint(), Tools::deg2rad(45.0), this->settings->zoom));
	Coordinate d(Tools::calculate(this->settings->get_centerpoint(), Tools::deg2rad(225.0), this->settings->zoom));
	
	this->min_lat = d.get_latitude();
	this->max_lat = c.get_latitude();
	
	this->min_lon = d.get_longitude();
	this->max_lon = c.get_longitude();
}

View::View(Drawsurface& d, std::shared_ptr <Settings> s) : drawer(d), settings(s) { }

View::~View() { }

void View::add_style(std::list <Style> tmp) {
	this->styles.splice(this->styles.end(), tmp);
}

void View::draw_menu(std::shared_ptr <Menu> menu) {
	Drawable_list menulist("ul", menu->get_class(), menu->get_id());
	
	std::vector <Menu_item> items = menu->get_items();
	
	std::vector <Menu_item> :: iterator it = items.begin();
	
	for (it = items.begin(); it != items.end(); ++it) {
		if (menu->get_selected().get_id() == (it->get_id())) {
			menulist.add_element(it->get_name(), "active");
		} else {
			menulist.add_element(it->get_name(), "normal");
		}
	}
	
	this->draw_element(menulist);
}

void View::load(std::string state) {
	#ifdef DEBUG
	std::clog << "View::load(" << state << ")" << std::endl;
	#endif
	
	this->paragraphs.clear();
	this->load_styles();
	this->load_layout(state);
	
	this->body.set_attribute("background-color", "rgb(0, 0, 0);");
	this->body = this->find_style("body");
}

/**
	* @TODO repair this as to read one .css file 
**/

void View::load_styles() {
	#ifdef DEBUG
	std::clog << "View::load_styles()" << std::endl;
	#endif
	
	DIR* dir;
	struct dirent *ent;
	
	dir = opendir(this->settings->style_folder.c_str());
	
	if (dir != NULL) {	
		#ifdef DEBUG
		std::clog << "View::Style folder opened" << std::endl;
		#endif
		while ((ent = readdir (dir)) != NULL) {
			std::string file_name = this->settings->style_folder + std::string(ent->d_name);
			
			if (file_name != "/." && file_name != "/..") {
				this->add_style(Tools::parse_css(file_name, this->settings));
			}
		}
		closedir (dir);
		
		return;
	} 
	
	throw std::logic_error("Directory not open");
}

/**
	* @TODO move this to layout ASAP
**/

void View::load_layout(std::string state) {
	#ifdef DEBUG
	std::clog << "View::load_layout(" << state << ")" << std::endl;
	#endif
	
	TiXmlDocument doc;
	bool load_ok = false;
	
	if (state == "game") {
		load_ok = doc.LoadFile(this->settings->layout_game_file_name.c_str());
	} else if (state == "atis") {
		load_ok = doc.LoadFile(this->settings->layout_atis_file_name.c_str());
	} else if (state == "stat") {
		load_ok = doc.LoadFile(this->settings->layout_stat_file_name.c_str());
	} else if (state == "menu") {
		load_ok = doc.LoadFile(this->settings->layout_menu_file_name.c_str());
	}	
	
	#ifdef DEBUG
	std::clog << "Layout file loaded" << std::endl;
	#endif
	
	if (!load_ok) {
		throw std::logic_error("layout-file " + state + ".xml not ok");
	} 
	
    TiXmlElement *root_element, *selected_element;
    root_element = doc.FirstChildElement("layout");
	
    if (root_element) {
        selected_element = root_element->FirstChildElement();
		
        while (selected_element) {
			if (selected_element->Value() == std::string("img")) {
				std::string image_id = Tools::trim(selected_element->Attribute("id"));
				std::string image_class = Tools::trim(selected_element->Attribute("class"));
				std::string image_name = selected_element->Value();
				
				std::string source = selected_element->Attribute("src");
				
				Image img(source, image_name, image_class, image_id);
				
				this->images.push_back(img);
			} else if (selected_element->Value() == std::string("p")) {
				std::string paragraph_id = Tools::trim(selected_element->Attribute("id"));
				std::string paragraph_class = Tools::trim(selected_element->Attribute("class"));
				std::string paragraph_name = selected_element->Value();
				
				std::string content = Tools::trim(selected_element->GetText());
			
				Paragraph p(content, paragraph_name, paragraph_class, paragraph_id);
				
				this->paragraphs.push_back(p);
			} else if (selected_element->Value() == std::string("table")) {
				std::string table_id = (selected_element->Attribute("id") == NULL) ? "" : Tools::trim(selected_element->Attribute("id"));
				std::string table_class = (selected_element->Attribute("class") == NULL) ? "" : Tools::trim(selected_element->Attribute("class"));
				std::string table_name = selected_element->Value();
				
				Drawable_table table(table_name, table_class, table_id);
				
				for (TiXmlElement* e = selected_element->FirstChildElement(); e != NULL; e = e->NextSiblingElement("tr")) {
					std::string row_id = (e->Attribute("id") == NULL) ? "-" : Tools::trim(e->Attribute("id"));
					std::string row_class = (e->Attribute("class") == NULL) ? "-" : Tools::trim(e->Attribute("class"));
					std::string row_name = e->Value();
					
					Row row(row_name, row_class, row_id);
					
					for (TiXmlElement* f = e->FirstChildElement(); f != NULL; f = f->NextSiblingElement()) {
						std::string cell_id = (f->Attribute("id") == NULL) ? "" : Tools::trim(f->Attribute("id"));
						std::string cell_class = (f->Attribute("class") == NULL) ? "" : Tools::trim(f->Attribute("class"));
						std::string cell_name = f->Value();
						std::string text = (f->GetText()) ? f->GetText() : "";
						
						Cell cell(text, cell_name, cell_class, cell_id);
						row.add_cell(cell);
					}
					
					table.add_row(row);
				}
				
				this->tables.push_back(table);
			} else if (selected_element->Value() == std::string("ul")) {
				std::string list_id = Tools::trim(selected_element->Attribute("id"));
				std::string list_class = Tools::trim(selected_element->Attribute("class"));
				std::string list_name = selected_element->Value();
				
				Drawable_list list(list_name, list_class, list_id);
				
				for (TiXmlElement* e = selected_element->FirstChildElement(); e != NULL; e = e->NextSiblingElement()) {
					std::string list_item_id = Tools::trim(e->Attribute("id"));
					std::string list_item_class = Tools::trim(e->Attribute("class"));
					
					list.add_element(e->GetText(), list_item_class, list_item_id);
				}
				
				this->lists.push_back(list);
			} else if (selected_element->Value() == std::string("click")) {
				std::string t_id = Tools::trim(selected_element->Attribute("id"));
				std::string t_class = Tools::trim(selected_element->Attribute("class"));
				std::string t_name = selected_element->Value();
				
				std::string value = Tools::trim(selected_element->GetText());
				
				Clickable cl(t_name, t_class, t_id, value, this->settings);
				
				this->clicks.push_back(cl);
			} 
			
            selected_element = selected_element->NextSiblingElement();
        }
	}
	
	#ifdef DEBUG
	std::clog << "Layout file parsed" << std::endl;
	#endif
	
}

void View::clear_screen() {
	#ifdef DEBUG
	std::clog << "View::clear_screen(" << body.get_background_color() << ")" << std::endl;
	#endif
	
	this->body = this->find_style("body");
	drawer.clear_screen(body.get_background_color());
}

void View::render() {
	#ifdef DEBUG
	std::clog << "View::render()" << std::endl;
	#endif
	
    drawer.flip();
}

void View::draw_borders(Style& style) {
	unsigned int color = style.get_border_color();
	int width = style.get_width();
	int height = style.get_height();
	int margin = style.get_margin();

	Point place_a = style.get_place();
	Point place_b(place_a.get_x(), place_a.get_y());
	
	place_b.change_x(width + margin);
	place_a.change_x(-margin);
	
	place_a.change_y(-margin);
	place_b.change_y(-margin);
	
	this->drawer.lineColor(place_a, place_b, color);
	
	place_a.change_y(margin + height + margin);
	place_b.change_y(margin + height + margin);
	
	this->drawer.lineColor(place_a, place_b, color);
	
	place_a.change_x(margin + width + margin);
	place_a.change_y(-margin -height - margin);
	
	this->drawer.lineColor(place_a, place_b, color);
	
	place_a.change_x(-margin - width - margin);
	place_b.change_x(-margin - width - margin);
	
	this->drawer.lineColor(place_a, place_b, color);
}

void View::draw_element(Image& img) {
	#ifdef DEBUG
	std::clog << "View::draw_element(Image& img)" << std::endl;
	#endif
	
	this->style(img);
	Point place = img.get_style().get_place();
	this->drawer.draw_picture(img.get_source(), place);
}

void View::draw_element(Paragraph& p) {
	#ifdef DEBUG
	std::clog << "View::draw_element(Paragraph& p) " << p.get_content() << " " << p.get_class() << std::endl;
	#endif
	
	this->style(p);
	
	Style style = p.get_style();
	int font_size = style.get_font_size();
	
	Point place_a = style.get_place();
	
	unsigned int width = this->drawer.get_text_length(p.get_content(), font_size);
	unsigned int height = 1;
	unsigned int t_height = 1;
	p.get_style().set_attribute("height", height);
	
	if (width <= style.get_width()) {
		this->draw_element(Tools::replace(p.get_content(), repl), place_a, style.get_text_color(), font_size);
		t_height = this->drawer.get_text_height(p.get_content(), font_size) + 5;
		height += t_height;
		place_a.change_y(t_height);
	} else {
		std::vector <std::string> tmp = Tools::split(" ", p.get_content());
		std::vector <std::string> lines;
		std::string line;
		std::string t_line;
		
		for (unsigned int i = 0; i < tmp.size(); ++i) {
			t_line += tmp[i] + " ";
			width = this->drawer.get_text_length(t_line, font_size);
			
			if (width > style.get_width()) {
				lines.push_back(line);
				line = "";
				t_line = tmp[i] + " ";
			} else {
				line = t_line;
			}
		}
		
		lines.push_back(t_line);
		height = lines.size();
		
		p.get_style().set_attribute("height", height);
		
		for (unsigned int i = 0; i < lines.size(); ++i) {
			this->draw_element(Tools::replace(lines[i], repl), place_a, style.get_text_color(), font_size);
			t_height = this->drawer.get_text_height(lines[i], font_size) + 5;
			height += t_height;
			place_a.change_y(t_height);
		}
	}
	
	p.get_style().set_attribute("height", height);
	this->draw_borders(p.get_style());
}

void View::draw_element(Drawable_Runway_Element& re) {
	#ifdef DEBUG
	std::clog << "View::draw_element(Drawable_Runway_Element& re)" << std::endl;
	#endif
	
	this->drawer.lineColor(re.get_begin(), re.get_end(), re.get_style().get_border_color());
	this->drawer.circleColor(re.get_approach(), 5, re.get_style().get_border_color());
}

void View::draw_element(Drawable_Navpoint_Element& ne) {
	#ifdef DEBUG
	std::clog << "View::draw_element(Drawable_Navpoint_Element& ne)" << std::endl;
	#endif
	
	Point place_screen = ne.get_place();
	
	this->drawer.trigonColor(place_screen, 15, ne.get_style().get_text_color(), ne.get_style().get_border_color());
	place_screen.change_x(15);
	place_screen.change_y(-10);
	this->drawer.draw_text(ne.get_name(), place_screen, ne.get_style().get_text_color());
}

void View::draw_element(std::shared_ptr <Drawable_input>& i) {
	#ifdef DEBUG
	std::clog << "View::draw_element(Drawable_input& i)" << std::endl;
	#endif

	Style style = i->get_style();
	int font_size = style.get_font_size();
		
	Point place_a = style.get_place();
		
	Point place_b(place_a.get_x() + style.get_width(), place_a.get_y() + style.get_height());
	
	this->drawer.rectangleColor(place_a, place_b, style.get_background_color(), style.get_border_color());
	this->draw_element(i->get_value(), place_a, style.get_text_color(), font_size);
}

void View::style(std::shared_ptr <Drawable_input>& de) {
	#ifdef DEBUG
	std::clog << "View::style(" << de->get_name() << ")" << std::endl;
	#endif
	
	Style style = find_style(de->get_name(), de->get_classes(), de->get_id());
	de->set_style(style);
}

Style View::find_style(std::string name, std::set <std::string> t_classes, std::string id) {
	#ifdef DEBUG
	std::clog << "View::find_style(" << name << ", " << t_classes.size() << ", " << id << ")" << std::endl;
	#endif
	Style style;
	std::list <Style> :: iterator t_style = this->styles.begin();
	
    while (t_style != this->styles.end()) {
		if (name.length() && t_style->get_name() == name) {
			style = *t_style;
		}
		
		++t_style;
	}
	
	t_style = this->styles.begin();
	
	while (t_style != this->styles.end()) {
		std::set <std::string> :: iterator it = t_classes.begin();
		
		while (it != t_classes.end()) {
			if ((*it).length() && t_style->get_class() == (*it)) {
				style = *t_style;
			}
			
			++it;
		}
		
		++t_style;
	}
	
	t_style = this->styles.begin();
	
	std::string tmp_id = id;
	
	while (t_style != this->styles.end()) {
		if (tmp_id.length() && t_style->get_id() == tmp_id) {
			style = *t_style;
		}
		
		++t_style;
	}
	
	return style;
}

void View::style(Drawable_element& de) {
	//std::clog << de.get_id() << " " << de.get_class() << std::endl;
	#ifdef DEBUG
	std::clog << "View::style(Drawable_element& de)" << std::endl;
	#endif
	
	Style style = find_style(de.get_name(), de.get_classes(), de.get_id());
	de.set_style(style);
}

void View::draw() {
	#ifdef DEBUG
	std::clog << "View::draw()" << std::endl;
	#endif
    std::vector <Image> :: iterator image;
    std::vector <Paragraph> :: iterator paragraph;
    std::vector <Drawable_table> :: iterator table;
    std::vector <Drawable_list> :: iterator list;
	
	for (image = this->images.begin(); image !=  this->images.end(); ++image) {
        draw_element(*image);
    }
	
	#ifdef DEBUG
	std::clog << "View::draw() -> images drawed" << std::endl;
	#endif
	
	for (paragraph = this->paragraphs.begin(); paragraph !=  this->paragraphs.end(); ++paragraph) {
        draw_element(*paragraph);
    }
	
	#ifdef DEBUG
	std::clog << "View::draw() -> paragraphs drawed" << std::endl;
	#endif
		
	for (table = this->tables.begin(); table !=  this->tables.end(); ++table) {
        draw_element(*table);
    }
	
	#ifdef DEBUG
	std::clog << "View::draw() -> tables drawed" << std::endl;
	#endif
	
	for (list = this->lists.begin(); list !=  this->lists.end(); ++list) {
        draw_element(*list);
    }
	
	#ifdef DEBUG
	std::clog << "View::draw() -> lists drawed" << std::endl;
	#endif
}

void View::draw_element(Drawable_list& dl) {
	this->style(dl);
	
	unsigned int color = dl.get_style().get_text_color();
	int font_size = dl.get_style().get_font_size();
	int height = 0;
	int t_height;
	
	std::list <Drawable_list_item> list_items = dl.get_elements();
	std::list <Drawable_list_item> :: iterator list_item = list_items.begin();
	
	Point place = dl.get_style().get_place();
	dl.get_style().set_attribute("width", this->drawer.get_text_length(dl.get_max_length(), font_size));
	
	for (list_item = list_items.begin(); list_item != list_items.end(); ++list_item) {
		this->style(*list_item);
		
		color = list_item->get_style().get_text_color();

		this->draw_element(Tools::replace(list_item->get_content(), repl), place, color, font_size);
		t_height = this->drawer.get_text_height(list_item->get_content(), font_size) + 5;
		height += t_height;
		place.change_y(t_height);
	}
	
	dl.get_style().set_attribute("height", height);
	
	this->draw_borders(dl.get_style());
}

void View::draw_element(Drawable_table& dt) {
	#ifdef DEBUG
	std::clog << "View::draw_element(Drawable_table& dt)" << std::endl;
	#endif
	
	this->style(dt);
	
	unsigned int color = dt.get_style().get_text_color();
	Point place = dt.get_style().get_place();
	int font_size = dt.get_style().get_font_size();
	
	std::list <Row> t_list = dt.get_rows();
	std::list <Row> :: iterator rit = t_list.begin();
	
	#ifdef DEBUG 
	std::clog << "Count of rows " <<  t_list.size() << std::endl;
	#endif
	
	int length = this->drawer.get_text_length(dt.get_max_length(), font_size) + 5;
	
	while (rit != t_list.end()) {
		this->style(*rit);
		
		std::list <Cell> c_list = rit->get_cells();
		std::list <Cell> :: iterator cit = c_list.begin();
		
		#ifdef DEBUG 
		std::clog << "Count of cells " <<  c_list.size() << std::endl;
		#endif
		
		//std::clog << "View::draw_element(row) " << rit->get_class() << std::endl;
		
		unsigned int r_color = rit->get_style().get_text_color();
			
		if (r_color > 0) {
			color = r_color;
		}
		
		while (cit != c_list.end()) {			
			this->style(*cit);
			
			#ifdef DEBUG
			std::clog << (*cit).get_class() << " " << (*cit).get_content() << std::endl;
			#endif
			
			unsigned int c_color = cit->get_style().get_text_color();
			
			if (c_color > 0) {
				color = c_color;
			}
			
			this->draw_element(Tools::replace((*cit).get_content(), repl), place, color, font_size);
			
			++cit;
			place.change_x(length);
		}
		
		place.change_x(-length * c_list.size());
		place.change_y(font_size);
		
		++rit;
	}
}

void View::draw_element(std::string text, Point& place_a, unsigned int color, int font_size) {
	#ifdef DEBUG
	std::clog << "View::draw_element(" << text << ", " << place_a << ", " << color << ")" << std::endl;
	#endif
	
	this->drawer.draw_text(text, place_a, color, font_size);
}

void View::flash_message(std::string message) {
	Paragraph p(message, "p", "message", "flash");

	this->draw_element(p);
}

Style View::find_style(std::string name) {
	std::list <Style> :: iterator searched = std::find(this->styles.begin(), this->styles.end(), name);
	
	if (searched != this->styles.end()) {
		return *searched;
	}
	
	throw std::runtime_error("Searched style name '" + name + "' not found");
}

double View::distanceNM(double pixels) {
	double center_w = this->settings->screen_width / 2.0;
	double center_h = this->settings->screen_height / 2.0;
	
	double center2corner = std::sqrt(std::pow(center_w, 2.0) + std::pow(center_h, 2.0));
	
	return (this->settings->zoom * pixels) / center2corner;
}

void View::set_menu(std::shared_ptr <Menu> m) { 
	this->menu = m;
}

void View::update_command(std::string) { }
