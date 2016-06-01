#include "view.hpp"

View::View(Drawsurface& d, Settings& s) : drawer(d), settings(s) { 
	this->styles = parse_css::parse("styles/style.css");
}

View::~View() { }

int count_childs(TiXmlNode* crnt) {
    int count = 0;

    for(TiXmlNode* child = crnt->FirstChild(); child; child = child->NextSibling() ) {
        count++;
    }

    return count;
}

void View::load() {
	std::clog << "View::load()" << std::endl;
    this->layout_elements.clear();
	
	TiXmlDocument doc(this->settings.layout_file_name.c_str());
	bool load_ok = doc.LoadFile();
	
	if (!load_ok) {
		throw std::logic_error("layout-file " + this->settings.layout_file_name + " not found");
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

void View::draw() {
    std::map <std::string, Layout_element> :: iterator element;

    for (element = layout_elements.begin(); element != layout_elements.end(); ++element) {
        style(element->second);
        draw_element(element->second);
    }
}

void View::add_element(std::string name, std::string id, std::string cl, std::string value) {
	if (this->layout_elements.find(name) == this->layout_elements.end()) {
		Layout_element le(name, id, cl);
		this->layout_elements[name] = le;
	} 
	
    this->layout_elements[name].set_content(value);
}
