#include "view.hpp"

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

Input::Input(std::string t_name, std::string t_class, std::string t_id) : Drawable_element(t_name, t_class, t_id) { }

std::string Input::get_content() {
	return this->content;
}

void Input::set_content(std::string cnt) {
	this->content = cnt;
}

View::View(Drawsurface& d, Settings& s) : drawer(d), settings(s) { 
	this->styles = parse_css::parse("styles/style.css");
}

View::~View() { }

void View::load() {
	std::clog << "View::load()" << std::endl;
    this->layout_elements.clear();
	
	TiXmlDocument doc(this->settings.layout_file_name.c_str());
	bool load_ok = doc.LoadFile();
	
	if (!load_ok) {
		throw std::logic_error("layout-file " + this->settings.layout_file_name + " not found");
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
				
				this->images.push_back(img);
			} else if (pParm->Value() == std::string("p")) {
				std::string t_id = Tools::trim(pParm->Attribute("id"));
				std::string t_class = Tools::trim(pParm->Attribute("class"));
				std::string t_name = pParm->Value();;
				
				std::string content = Tools::trim(pParm->GetText());
			
				Paragraph p(content, t_name, t_class, t_id);
			
				this->paragraphs.push_back(p);
			} else if (pParm->Value() == std::string("input")) {
				std::string id = Tools::trim(pParm->Attribute("id"));
			}
			
            pParm = pParm->NextSiblingElement();
            ++i;
        }
	}

    std::map <std::string, Layout_element> :: iterator le;
	
    for (le = this->layout_elements.begin(); le != this->layout_elements.end(); ++le) {
        //this->style(le->second);
    }
}

void View::clear_screen() {
    drawer.clear_screen();
}

void View::render() {
    drawer.flip();
}

void View::draw_element(Layout_element& layout_element) {
	if (layout_element.b_color_setted) {
        drawer.rectangleColor(layout_element.get_top_left(), layout_element.get_bottom_right(), layout_element.b_red, layout_element.b_green, layout_element.b_blue, true);
    } else {
        drawer.rectangleColor(layout_element.get_top_left(), layout_element.get_bottom_right(), "black", true);
    }

    Point t = layout_element.get_top_left();
    t.change_y(-8);

    if (layout_element.t_color_setted) {
        drawer.draw_text(layout_element.get_name(), t, layout_element.t_red, layout_element.t_green, layout_element.t_blue, 10);
    } else {
        drawer.draw_text(layout_element.get_name(), t, "blue", 10);
    }

    t.change_y(8);

    std::vector <std::string> content = layout_element.get_content();
	
    for (unsigned int i = 0; i < content.size(); ++i) {
	   if (layout_element.t_color_setted) {
            drawer.draw_text(content[i], t, layout_element.t_red, layout_element.t_green, layout_element.t_blue);
        } else {
            drawer.draw_text(content[i], t, "blue");
        }

        t.change_y(drawer.get_fontsize());
    }
}

void View::draw_element(Image& img) {
	Point place(15, 60);
	this->drawer.draw_picture(img.get_source(), place);
}

void View::draw_element(Paragraph& p) {
	this->style(p);
		
	int color = p.get_style("text-color");
	int background_color = p.get_style("background-color");
	
	Point place_a(p.get_style("left"), p.get_style("top"));
	
	if (background_color > 0) {
		Point place_b(place_a.get_x() + p.get_style("width"), place_a.get_y() + p.get_style("height"));
		this->drawer.rectangleColor(place_a, place_b, background_color, false);
	}
	
	this->drawer.draw_text(Tools::replace(p.get_content(), repl), place_a, color);
}

bool compare_length(std::string const& lhs, std::string const& rhs) {
    return lhs.size() < rhs.size();
}
/*
void View::style(Layout_element& le) {
    std::list <Style> :: iterator t_style = this->styles.begin();
	
    while (t_style != this->styles.end()) {
		
        ++t_style;
    }
}
*/
void View::style(Paragraph& p) {
    std::list <Style> :: iterator t_style = this->styles.begin();
	
    while (t_style != this->styles.end()) {
		if (t_style->get_name() == p.get_name() || t_style->get_class() == p.get_class() || t_style->get_id() == p.get_id()) {
			p.set_style((*t_style));
		}
		
		++t_style;
	}
}

void View::draw() {
    std::map <std::string, Layout_element> :: iterator element;
    std::vector <Image> :: iterator image;
    std::vector <Paragraph> :: iterator paragraph;
 
    for (element = layout_elements.begin(); element != layout_elements.end(); ++element) {
        draw_element(element->second);
    }
	
	for (image = this->images.begin(); image !=  this->images.end(); ++image) {
        draw_element(*image);
    }
	
	for (paragraph = this->paragraphs.begin(); paragraph !=  this->paragraphs.end(); ++paragraph) {
        draw_element(*paragraph);
    }
}

void View::add_element(std::string name, std::string id, std::string cl, std::string value) {
	if (this->layout_elements.find(name) == this->layout_elements.end()) {
		Layout_element le(name, id, cl);
		this->layout_elements[name] = le;
	} 
	
    this->layout_elements[name].set_content(value);
}
