#include "view.hpp"

Drawable_input::Drawable_input(std::string val, std::string t_name, std::string t_class, std::string t_id) : Drawable_element(t_name, t_class, t_id), value(val) { }

void Drawable_input::set_value(std::string val) {
	this->value = val;
}

std::string Drawable_input::get_value() {
	return this->value;
}

Drawable_list::Drawable_list(std::string t_name, std::string t_class, std::string t_id) : Drawable_element(t_name, t_class, t_id) { }

void Drawable_list::add_element(std::string content) {
	this->elements.push_back(content);
}

void Drawable_list::clear_content() {
	this->elements.clear();
}

std::list <std::string> Drawable_list::get_elements() {
	return this->elements;
}

Image::Image(std::string src, std::string t_name, std::string t_class, std::string t_id) : Drawable_element(t_name, t_class, t_id), source(src) { }

std::string Image::get_source() {
	return this->source;
}
	
void Image::set_source(std::string src) {
	this->source = src;
}

Paragraph::Paragraph(std::string cnt, std::string t_name, std::string t_class, std::string t_id) : Drawable_element(t_name, t_class, t_id), content(cnt) { }

std::string Paragraph::get_content() {
	return this->content;
}

void Paragraph::set_content(std::string cnt) {
	this->content = cnt;
}

View::View(Drawsurface& d, Settings& s) : drawer(d), settings(s) { 
	this->styles = parse_css::parse("styles/style.css");
}

View::~View() { }

void View::load(std::string state) {
	std::clog << "View::load()" << std::endl;
	
	std::string file_name;
	
	TiXmlDocument doc;
	bool load_ok; 
	
	if (state == "game") {
		file_name = this->settings.layout_game_file_name;
		load_ok = doc.LoadFile(file_name.c_str());
	} else if (state == "atis") {
		file_name = this->settings.layout_atis_file_name;
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
				
				Image img(t_name, t_class, t_id, source);
				
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
			} 
			
            pParm = pParm->NextSiblingElement();
            ++i;
        }
	}
}

void View::clear_screen() {
    drawer.clear_screen();
}

void View::render() {
    drawer.flip();
}

void View::draw_element(Image& img) {
	Point place = img.get_style().get_place();
	this->drawer.draw_picture(img.get_source(), place);
}

void View::draw_element(Paragraph& p) {
	Style st = p.get_style();
	int color = st.get_text_color();
	int background_color = st.get_background_color();
	
	Point place_a = st.get_place();
	
	if (background_color > 0) {
		Point place_b(place_a.get_x() + st.get_width(), place_a.get_y() + st.get_height());
		this->drawer.rectangleColor(place_a, place_b, background_color);
	}
	
	this->drawer.draw_text(Tools::replace(p.get_content(), repl), place_a, color);
}

void View::draw_element(Drawable_input& i) {
	Style st = i.get_style();
	int color = st.get_text_color();
	int background_color = st.get_background_color();
	
	Point place_a = st.get_place();
	
	if (background_color > 0) {
		Point place_b(place_a.get_x() + st.get_width(), place_a.get_y() + st.get_height());
		this->drawer.rectangleColor(place_a, place_b, background_color);
	}
	
	this->drawer.draw_text(i.get_value(), place_a, color);
}

void View::style(Drawable_element& de) {
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
    std::vector <Image> :: iterator image;
    std::vector <Paragraph> :: iterator paragraph;
    std::vector <Drawable_input> :: iterator input;
	
	for (image = this->images.begin(); image !=  this->images.end(); ++image) {
        draw_element(*image);
    }
	
	for (paragraph = this->paragraphs.begin(); paragraph !=  this->paragraphs.end(); ++paragraph) {
        draw_element(*paragraph);
    }
	
	for (input = this->inputs.begin(); input !=  this->inputs.end(); ++input) {
        draw_element(*input);
    }
}

void View::draw_element(Drawable_list& dl) {
	int color = dl.get_style().get_text_color();
	
	std::list <std::string> t_list = dl.get_elements();
	std::list <std::string> :: iterator it = t_list.begin();
	
	Point place = dl.get_style().get_place();

	for (it = t_list.begin(); it != t_list.end(); ++it) {
		this->drawer.draw_text((*it), place, color);
		place.change_y(20);
	}
}