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

void View::load(std::string state) {
	#ifdef DEBUG
	std::clog << "View::load(" << state << ")" << std::endl;
	#endif
	this->paragraphs.clear();
	this->load_styles();
	this->load_layout(state);
	
	try {
		this->body = this->find_style("body");
	} catch (std::runtime_error& re) {
		std::cerr << re.what() << std::endl;
		this->body.set_attribute("background-color", 0);
	}
}

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
				this->add_style(this->parse_css(file_name));
			}
		}
		closedir (dir);
		
		return;
	} 
	
	throw std::logic_error("Directory not open");
}

std::map <std::string, std::string> View::get_info(TiXmlElement *selected_element) {
	std::map <std::string, std::string> tmp;

	tmp["id"] = selected_element->Attribute("id");
	tmp["class"] = selected_element->Attribute("class");
	
	return tmp;
}

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
		std::map <std::string, std::string> element_info;
		
        while (selected_element) {
			if (selected_element->Value() == std::string("img")) {
				element_info = this->get_info(selected_element);
				std::string t_name = selected_element->Value();
				std::list <std::string> t_class;
				std::vector <std::string> t_explode =  Tools::split(" ", element_info["class"]);
				
				std::copy(t_explode.begin(), t_explode.end(), std::back_inserter(t_class));
				
				std::string source = selected_element->Attribute("src");
				
				Image img(source, t_name, t_class, element_info["id"]);
				
				this->style(img);
				this->images.push_back(img);
			} else if (selected_element->Value() == std::string("p")) {
				std::string paragraph_id = Tools::trim(selected_element->Attribute("id"));
				std::string paragraph_class = Tools::trim(selected_element->Attribute("class"));
				std::string paragraph_name = selected_element->Value();
				
				std::string content = Tools::trim(selected_element->GetText());
			
				Paragraph p(content, paragraph_name, paragraph_class, paragraph_id);
				
				this->style(p);
				this->paragraphs.push_back(p);
			} else if (selected_element->Value() == std::string("table")) {
				std::string table_id = (selected_element->Attribute("id") == NULL) ? "" : Tools::trim(selected_element->Attribute("id"));
				std::string table_class = (selected_element->Attribute("class") == NULL) ? "" : Tools::trim(selected_element->Attribute("class"));
				std::string table_name = selected_element->Value();
				
				Drawable_table table(table_name, table_class, table_id);
				this->style(table);
				this->tables.push_back(table);
				
				for (TiXmlElement* e = selected_element->FirstChildElement(); e != NULL; e = e->NextSiblingElement("tr")) {
					std::string row_id = (e->Attribute("id") == NULL) ? "" : Tools::trim(e->Attribute("id"));
					std::string row_class = (e->Attribute("class") == NULL) ? "" : Tools::trim(e->Attribute("class"));
					std::string row_name = e->Value();
					
					Row row(row_name, row_class, row_id);
					this->tables.back().add_row(row);
					
					for (TiXmlElement* f = e->FirstChildElement(); f != NULL; f = f->NextSiblingElement()) {
						std::string cell_id = (f->Attribute("id") == NULL) ? "" : Tools::trim(f->Attribute("id"));
						std::string cell_class = (f->Attribute("class") == NULL) ? "" : Tools::trim(f->Attribute("class"));
						std::string cell_name = f->Value();
						
						Cell cell(f->GetText(), cell_name, cell_class, cell_id);
						this->tables.back().add_cell(cell);
					}
				}
			} else if (selected_element->Value() == std::string("ul")) {
				std::string t_id = Tools::trim(selected_element->Attribute("id"));
				std::string t_class = Tools::trim(selected_element->Attribute("class"));
				std::string t_name = selected_element->Value();
				
				Drawable_list list(t_name, t_class, t_id);
				
				for (TiXmlElement* e = selected_element->FirstChildElement(); e != NULL; e = e->NextSiblingElement()) {
					std::string t_id1 = Tools::trim(e->Attribute("id"));
					std::string t_class1 = Tools::trim(e->Attribute("class"));
					
					list.add_element(e->GetText(), t_class1, t_id1);
				}
				
				this->style(list);
				this->lists.push_back(list);
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
	
	drawer.clear_screen(body.get_background_color());
}

void View::render() {
	#ifdef DEBUG
	std::clog << "View::render()" << std::endl;
	#endif
    drawer.flip();
}

void View::draw_element(Image& img) {
	#ifdef DEBUG
	std::clog << "View::draw_element(Image& img)" << std::endl;
	#endif
	
	Point place = img.get_style().get_place();
	this->drawer.draw_picture(img.get_source(), place);
}

void View::draw_borders(Style& style) {
	int color = style.get_border_color();
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

void View::draw_element(Paragraph& p) {
	#ifdef DEBUG
	std::clog << "View::draw_element(Paragraph& p) " << p.get_content() << " " << p.get_class() << std::endl;
	#endif
	
	Style st = p.get_style();
	int color = st.get_text_color();
	
	Point place_a = st.get_place();
	
	unsigned int width = this->drawer.get_text_length(p.get_content(), 16);
	unsigned int height = 1;
	unsigned int t_height = 1;
	p.get_style().set_attribute("height", height);
	
	if (width <= st.get_width()) {
		this->draw_element(Tools::replace(p.get_content(), repl), place_a, color);
		t_height = this->drawer.get_text_height(p.get_content(), 16) + 5;
		height += t_height;
		place_a.change_y(t_height);
	} else {
		std::vector <std::string> tmp = Tools::split(" ", p.get_content());
		std::vector <std::string> lines;
		std::string line;
		std::string t_line;
		
		for (unsigned int i = 0; i < tmp.size(); ++i) {
			t_line += tmp[i] + " ";
			width = this->drawer.get_text_length(t_line, 16);
			
			if (width > st.get_width()) {
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
			this->draw_element(Tools::replace(lines[i], repl), place_a, color);
			t_height = this->drawer.get_text_height(lines[i], 16) + 5;
			height += t_height;
			place_a.change_y(t_height);
		}
	}
	
	p.get_style().set_attribute("height", height);
	this->draw_borders(p.get_style());
}

void View::draw_element(std::shared_ptr <Drawable_input>& i) {
	#ifdef DEBUG
	std::clog << "View::draw_element(Drawable_input& i)" << std::endl;
	#endif

	Style st = i->get_style();
	
	int color = st.get_text_color();
	int background_color = st.get_background_color();
	
	Point place_a = st.get_place();
		
	Point place_b(place_a.get_x() + st.get_width(), place_a.get_y() + st.get_height());
	
	this->drawer.rectangleColor(place_a, place_b, background_color);
	this->draw_element(i->get_value(), place_a, color);
}

void View::style(std::shared_ptr <Drawable_input>& de) {
	#ifdef DEBUG
	std::clog << "View::style(" << de->get_name() << ")" << std::endl;
	#endif
	
	Style style = find_style(de->get_name(), de->get_classes(), de->get_id());
	de->set_style(style);
}

Style View::find_style(std::string name, std::list <std::string> t_classes, std::string id) {
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
		std::list <std::string> :: iterator it = t_classes.begin();
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

void View::draw(Point&) { }

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
	#ifdef DEBUG
	std::clog << "View::draw_element(Drawable& dl)" << std::endl;
	#endif
	
	int color = dl.get_style().get_text_color();
	int height = 0;
	int t_height;
	
	std::list <Drawable_list_item> t_list = dl.get_elements();
	std::list <Drawable_list_item> :: iterator it = t_list.begin();
	
	Point place = dl.get_style().get_place();
	dl.get_style().set_attribute("width", this->drawer.get_text_length(dl.get_max_length(), 16));
	
	for (it = t_list.begin(); it != t_list.end(); ++it) {
		this->style(*it);
		color = it->get_style().get_text_color();
		//std::clog << it->get_content() << " color " << color << std::endl;
		//std::clog << it->get_style() << std::endl;
		this->draw_element(Tools::replace(it->get_content(), repl), place, color);
		t_height = this->drawer.get_text_height(it->get_content(), 16) + 5;
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
	
	int color = dt.get_style().get_text_color();
	Point place = dt.get_style().get_place();
	int font_size = this->drawer.get_fontsize() + 5;
	
	std::list <Row> t_list = dt.get_rows();
	std::list <Row> :: iterator rit = t_list.begin();
	
	#ifdef DEBUG 
	std::clog << "Count of rows " <<  t_list.size() << std::endl;
	#endif
	
	int length = this->drawer.get_text_length(dt.get_max_length(), 16) + 15;
	
	while (rit != t_list.end()) {
		std::list <Cell> c_list = rit->get_cells();
		std::list <Cell> :: iterator cit = c_list.begin();
		
		#ifdef DEBUG 
		std::clog << "Count of cells " <<  c_list.size() << std::endl;
		#endif
		
		int r_color = rit->get_style().get_text_color();
			
		if (r_color > 0) {
			color = r_color;
		}
		
		while (cit != c_list.end()) {
			this->style(*cit);
			#ifdef DEBUG
			std::clog << (*cit).get_content() << std::endl;
			#endif
			int c_color = cit->get_style().get_text_color();
			
			if (c_color > 0) {
				color = c_color;
			}
		
			this->draw_element(Tools::replace((*cit).get_content(), repl), place, color);
			++cit;
			place.change_x(length);
			color = 0;
		}
		
		place.change_x(-length * c_list.size());
		place.change_y(font_size);
		
		++rit;
	}
}

std::list <Style> View::parse_css(std::string file) {
	std::ifstream fin(file.c_str(), std::ios::in);
    std::string line;
    std::string id;
    std::string s_class;
    std::string name;
	int line_number = 1;
	int block_open = 0;
	
    std::list <Style> tmp;

    while (std::getline(fin, line)) {
		size_t found = line.find("{");
		
        if (found != std::string::npos) {
			++block_open;
			
			Style t(this->settings);
			
			if (line.substr(0, 1) == "#") {
				id = line.substr(1, found-1);
				id = Tools::trim(id);
				t.set_attribute("id", id);
			} else if (line.substr(0, 1) == ".") {
				s_class = line.substr(1, found-1);
				s_class = Tools::trim(s_class);
				t.set_attribute("class", s_class);
			} else {
				name = line.substr(0, found-1);
				name = Tools::trim(name);
				t.set_attribute("name", name);
			}
			
			tmp.push_back(t);
        } else if (line.substr(0, 1) != "}" && line != "") {
			size_t found = line.find(":");
			if (found != std::string::npos) {
				std::string key     = Tools::trim(line.substr(0, line.find(":")));
				std::string value   = Tools::trim(line.substr(line.find(":")+1));

				tmp.back().set_attribute(key, value);
			} else {
				throw std::logic_error("Unexpected style in file " + file + " line " + Tools::tostr(line_number));
			}
		} else if (line.substr(0, 1) == "}") {
			--block_open;
			if (block_open != 0) {
				throw std::logic_error("css file " + file + " not well formed. Before line " + Tools::tostr(line_number));
			}
		}
		
		++line_number;
    }

    fin.close();

    return tmp;
}

void View::draw_element(std::string text, Point& place_a, int color) {
	#ifdef DEBUG
	std::clog << "View::draw_element(" << text << ", " << place_a << ", " << color << ")" << std::endl;
	#endif
	this->drawer.draw_text(text, place_a, color);
}

void View::flash_message(std::string message) {
	Paragraph p(message, "p", "message", "flash");
	this->style(p);
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
