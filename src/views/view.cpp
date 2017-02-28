#include "view.hpp"

View::View(Drawsurface& d, Settings& s) : drawer(d), settings(s) { }

View::~View() { }

void View::add_style(std::list <Style> tmp) {
	this->styles.splice(this->styles.end(), tmp);
}

void View::load(std::string state) {
	DIR *dir;
	struct dirent *ent;
	if ((dir = opendir (this->settings.style_folder.c_str())) != NULL) {
		while ((ent = readdir (dir)) != NULL) {
			std::string file_name = this->settings.style_folder + std::string(ent->d_name);
			if (file_name != "/." && file_name != "/..") {
				this->add_style(parse_css::parse(file_name));
			}
		}
		closedir (dir);
	} else {
		std::cerr << "Directory not open" << std::endl;
	}
	
	std::string file_name;
	
	TiXmlDocument doc;
	bool load_ok = false;
	
	if (state == "game") {
		file_name = this->settings.layout_game_file_name;
		load_ok = doc.LoadFile(file_name.c_str());
	} else if (state == "atis") {
		file_name = this->settings.layout_atis_file_name;
		load_ok = doc.LoadFile(file_name.c_str());
	} else if (state == "stat") {
		file_name = this->settings.layout_stat_file_name;
		load_ok = doc.LoadFile(file_name.c_str());
	}	
	
	if (!load_ok) {
		throw std::logic_error("layout-file " + file_name + " not found");
	} 
	
    TiXmlElement *pRoot, *pParm;
    pRoot = doc.FirstChildElement("layout");
	
    if (pRoot) {
        pParm = pRoot->FirstChildElement();
        int i = 0; // for sorting the entries
		
        while (pParm) {
			if (pParm->Value() == std::string("img")) {
				std::string source = pParm->Attribute("src");
				std::string t_id = pParm->Attribute("id");
				std::string t_class = pParm->Attribute("class");
				std::string t_name = pParm->Value();
				
				Image img(source, t_name, t_class, t_id);
				
				this->style(img);
				this->images.push_back(img);
			} else if (pParm->Value() == std::string("p")) {
				std::string t_id = Tools::trim(pParm->Attribute("id"));
				std::string t_class = Tools::trim(pParm->Attribute("class"));
				std::string t_name = pParm->Value();
				
				std::string content = Tools::trim(pParm->GetText());
			
				Paragraph p(content, t_name, t_class, t_id);
				
				this->style(p);
				this->paragraphs.push_back(p);
			} else if (pParm->Value() == std::string("input")) {
				std::string t_id = Tools::trim(pParm->Attribute("id"));
				std::string t_class = Tools::trim(pParm->Attribute("class"));
				std::string t_name = pParm->Value();
				
				Drawable_input input("", t_name, t_class, t_id);
				
				this->style(input);
				this->inputs.push_back(input);
			} else if (pParm->Value() == std::string("table")) {
				std::string t_id = Tools::trim(pParm->Attribute("id"));
				std::string t_class = Tools::trim(pParm->Attribute("class"));
				std::string t_name = pParm->Value();
				
				Drawable_table table(t_name, t_class, t_id);
				this->style(table);
				this->tables.push_back(table);
				
				for (TiXmlElement* e = pParm->FirstChildElement(); e != NULL; e = e->NextSiblingElement("tr")) {
					Row row;
					this->tables.back().add_row(row);
					
					for (TiXmlElement* f = e->FirstChildElement(); f != NULL; f = f->NextSiblingElement()) {
						Cell cell(f->GetText());
						this->tables.back().add_cell(cell);
					}
				}
			} else if (pParm->Value() == std::string("ul")) {
				std::string t_id = Tools::trim(pParm->Attribute("id"));
				std::string t_class = Tools::trim(pParm->Attribute("class"));
				std::string t_name = pParm->Value();
				
				Drawable_list list(t_name, t_class, t_id);
				
				for (TiXmlElement* e = pParm->FirstChildElement(); e != NULL; e = e->NextSiblingElement()) {
					list.add_element(e->GetText());
				}
				
				this->style(list);
				this->lists.push_back(list);
			} 
			
            pParm = pParm->NextSiblingElement();
            ++i;
        }
	}
}

void View::clear_screen() {
	#ifdef DEBUG
	std::clog << "View::clear_screen()" << std::endl;
	#endif
    drawer.clear_screen();
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

void View::draw_element(Paragraph& p) {
	#ifdef DEBUG
	std::clog << "View::draw_element(Paragraph& p)" << std::endl;
	#endif
	Style st = p.get_style();
	int color = st.get_text_color();
	
	Point place_a = st.get_place();
	
	unsigned int width = this->drawer.get_text_length(p.get_content());
	
	if (width <= st.get_width()) {
		this->drawer.draw_text(Tools::replace(p.get_content(), repl), place_a, color);
	} else {
		std::vector <std::string> tmp = Tools::split(" ", p.get_content());
		std::vector <std::string> lines;
		std::string line;
		std::string t_line;
		
		for (unsigned int i = 0; i < tmp.size(); ++i) {
			t_line += tmp[i] + " ";
			width = this->drawer.get_text_length(t_line);
			
			if (width > st.get_width()) {
				lines.push_back(line);
				line = "";
				t_line = tmp[i] + " ";
			} else {
				line = t_line;
			}
		}
		
		lines.push_back(t_line);
		
		for (unsigned int i = 0; i < lines.size(); ++i) {
			this->drawer.draw_text(Tools::replace(lines[i], repl), place_a, color);
			place_a.change_y(25);
		}
		
	}
}

void View::draw_element(Drawable_input& i) {
	#ifdef DEBUG
	std::clog << "View::draw_element(Drawable_input& i)" << std::endl;
	#endif
	Style st = i.get_style();
	int color = st.get_text_color();
	int background_color = st.get_background_color();
	
	Point place_a = st.get_place();
	
	Point place_b(place_a.get_x() + st.get_width(), place_a.get_y() + st.get_height());
	this->drawer.rectangleColor(place_a, place_b, background_color);
	
	this->drawer.draw_text(i.get_value(), place_a, color);
}

void View::style(Drawable_element& de) {
	#ifdef DEBUG
	std::clog << "View::style(Drawable_element& de)" << std::endl;
	#endif
    std::list <Style> :: iterator t_style = this->styles.begin();
	
    while (t_style != this->styles.end()) {
		if (de.get_name().length() && t_style->get_name() == de.get_name()) {
			de.set_style((*t_style));
		}
		
		++t_style;
	}
	
	t_style = this->styles.begin();
	
	while (t_style != this->styles.end()) {
		if (de.get_class().length() && t_style->get_class() == de.get_class()) {
			de.set_style((*t_style));
		}
		
		++t_style;
	}
	
	t_style = this->styles.begin();
	
	while (t_style != this->styles.end()) {
		if (de.get_id().length() && t_style->get_id() == de.get_id()) {
			de.set_style((*t_style));
		}
		
		++t_style;
	}
}

void View::draw() {
	#ifdef DEBUG
	std::clog << "View::draw()" << std::endl;
	#endif
    std::vector <Image> :: iterator image;
    std::vector <Paragraph> :: iterator paragraph;
    std::vector <Drawable_input> :: iterator input;
    std::vector <Drawable_table> :: iterator table;
    std::vector <Drawable_list> :: iterator list;
	
	for (image = this->images.begin(); image !=  this->images.end(); ++image) {
        draw_element(*image);
    }
	
	for (paragraph = this->paragraphs.begin(); paragraph !=  this->paragraphs.end(); ++paragraph) {
        draw_element(*paragraph);
    }
	
	for (input = this->inputs.begin(); input !=  this->inputs.end(); ++input) {
        draw_element(*input);
    }
	
	for (table = this->tables.begin(); table !=  this->tables.end(); ++table) {
        draw_element(*table);
    }
	
	for (list = this->lists.begin(); list !=  this->lists.end(); ++list) {
        draw_element(*list);
    }
}

void View::draw_element(Drawable_list& dl) {
	#ifdef DEBUG
	std::clog << "View::draw_element(Drawable& dl)" << std::endl;
	#endif
	int color = dl.get_style().get_text_color();
	int font_size = this->drawer.get_fontsize() + 5;
	std::list <std::string> t_list = dl.get_elements();
	std::list <std::string> :: iterator it = t_list.begin();
	
	Point place = dl.get_style().get_place();
	
	for (it = t_list.begin(); it != t_list.end(); ++it) {
		this->drawer.draw_text((*it), place, color);
		place.change_y(font_size);
	}
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
	
	int length = this->drawer.get_text_length(dt.get_max_length()) + 15;
	
	while (rit != t_list.end()) {
		std::list <Cell> c_list = rit->get_cells();
		std::list <Cell> :: iterator cit = c_list.begin();
		
		int t_color = rit->get_style().get_text_color();
			
		if (t_color > 0) {
			color = t_color;
		}
		
		while (cit != c_list.end()) {
			this->drawer.draw_text(Tools::replace((*cit).get_content(), repl), place, color);
			++cit;
			place.change_x(length);
		}
		
		place.change_x(-length * c_list.size());
		place.change_y(font_size);
		
		++rit;
	}
}