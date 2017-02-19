#include "view.hpp"

View::View(Drawsurface& d, Settings& s) : drawer(d), settings(s) { }

View::~View() { }

void View::load(std::string state) {
	std::clog << "View::load()" << std::endl;
	this->styles = parse_css::parse("styles/style.css");
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
	int background_color = st.get_background_color();
	
	Point place_a = st.get_place();
	
	Point place_b(place_a.get_x() + st.get_width(), place_a.get_y() + st.get_height());
	this->drawer.rectangleColor(place_a, place_b, background_color);
	
	this->drawer.draw_text(Tools::replace(p.get_content(), repl), place_a, color);
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
		if (t_style->get_name() == de.get_name()) {
			de.set_style((*t_style));
		}
		
		++t_style;
	}
	
	t_style = this->styles.begin();
	
	while (t_style != this->styles.end()) {
		if (t_style->get_class() == de.get_class()) {
			de.set_style((*t_style));
		}
		
		++t_style;
	}
	
	t_style = this->styles.begin();
	
	while (t_style != this->styles.end()) {
		if (t_style->get_id() == de.get_id()) {
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
		//std::clog << "Draw " << (*it) << " " << place << " " << color << std::endl;
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
	
	while (rit != t_list.end()) {
		std::list <Cell> c_list = rit->get_cells();
		std::list <Cell> :: iterator cit = c_list.begin();
		
		while (cit != c_list.end()) {
			//std::clog << (*cit).get_content() << " " << place << " " << color << std::endl;

			this->drawer.draw_text(Tools::replace((*cit).get_content(), repl), place, color);
			++cit;
			place.change_x(160);
		}
		
		place.change_x(-160 * c_list.size());
		place.change_y(font_size);
		
		++rit;
	}
}