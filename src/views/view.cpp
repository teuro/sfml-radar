#include "view.hpp"

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
				std::string src = pParm->Attribute("src");
				std::string id = pParm->Attribute("id");
				Point place(0, 0);
				
				struct Image img = {src, id, place};
				
				this->images.push_back(img);
			} else if (pParm->Value() == std::string("p")) {
				std::string id = pParm->Attribute("id");
				std::string c = pParm->GetText();
				Point place(0, 0);
				
				Paragraph p = {c, id, place};
				this->paragraphs.push_back(p);
			}
			
            pParm = pParm->NextSiblingElement();
            ++i;
        }
	}

    std::map <std::string, Layout_element> :: iterator le;
	
    for (le = this->layout_elements.begin(); le != this->layout_elements.end(); ++le) {
        this->style(le->second);
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
	this->drawer.draw_picture(img.source, img.place);
}

void View::draw_element(Paragraph& p) {
	this->drawer.draw_text(Tools::replace(p.content, repl), p.place, "red");
}

bool compare_length(std::string const& lhs, std::string const& rhs) {
    return lhs.size() < rhs.size();
}

void View::style(Layout_element& le) {
    std::list <Style> :: iterator t_style = this->styles.begin();
    while (t_style != this->styles.end()) {
		if (le.get_id() == t_style->get_id()) {
	        Point p(t_style->get_left(), t_style->get_top());
            le.set_place(p);

            if (t_style->get_t_color() >= 0) {
				le.set_text_colors(t_style->get_t_color());
			}

            if (t_style->get_b_color() >= 0) {
                le.set_background_colors(t_style->get_b_color());
            }

            if (le.get_content().size() * drawer.get_fontsize() > t_style->get_height()) {
                t_style->set_height(le.get_content().size() * drawer.get_fontsize() + 30);
            }

            std::vector <std::string> lines = le.get_content();
            std::string longest_line;
            std::vector <std::string> :: iterator longest;

            if (lines.size() == 0) {
                longest_line = "";
            } else if (lines.size() == 1) {
                longest_line = lines[0];
            } else {
                longest = std::max_element(lines.begin(), lines.end(), compare_length);
                longest_line = *longest;
            }
			
            if (longest_line.length() * drawer.get_fontsize() > t_style->get_width()) {
                t_style->set_width(longest_line.length() * drawer.get_fontsize()/1.75);
            }

            le.set_size(t_style->get_width(), t_style->get_height());
        }

        ++t_style;
    }
}

void View::style(Image& img) {
    std::list <Style> :: iterator t_style = this->styles.begin();
	
    while (t_style != this->styles.end()) {
		if (img.id == t_style->get_id()) {
	        Point p(t_style->get_left(), t_style->get_top());
            img.place = p;
        }

        ++t_style;
    }
}

void View::style(Paragraph& p) {
    std::list <Style> :: iterator t_style = this->styles.begin();
	
    while (t_style != this->styles.end()) {
		if (p.id == t_style->get_id()) {
	        Point pl(t_style->get_left(), t_style->get_top());
            p.place = pl;
        }

        ++t_style;
    }
}

void View::draw() {
	this->repl["[RQD]"] = Tools::tostr(this->settings.required_handled);
		
    std::map <std::string, Layout_element> :: iterator element;
 
    for (element = layout_elements.begin(); element != layout_elements.end(); ++element) {
        style(element->second);
        draw_element(element->second);
    }
	
	for (unsigned int i = 0; i < this->images.size(); ++i) {
		style(this->images[i]);
        draw_element(this->images[i]);
    }
	
	for (unsigned int i = 0; i < this->paragraphs.size(); ++i) {
		style(this->paragraphs[i]);
        draw_element(this->paragraphs[i]);
    }
}

void View::add_element(std::string name, std::string id, std::string cl, std::string value) {
	if (this->layout_elements.find(name) == this->layout_elements.end()) {
		Layout_element le(name, id, cl);
		this->layout_elements[name] = le;
	} 
	
    this->layout_elements[name].set_content(value);
}
