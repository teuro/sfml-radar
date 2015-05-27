#include "view.hpp"

View::View(Drawsurface& d) : drawer(d) { }

View::~View() { }

int count_childs(TiXmlNode* crnt) {
    int count = 0;

    for(TiXmlNode* child = crnt->FirstChild(); child; child = child->NextSibling() ) {
        count++;
    }

    return count;
}

void View::iterate(TiXmlNode* el) {
    for (TiXmlNode* node = el->FirstChild(); node; node = node->NextSibling()) {
        std::string node_name = node->Value();

        if (node_name == "element") {
            TiXmlElement* pr = node->ToElement();

            if (pr->Attribute("id") && pr->Attribute("class") && pr->Attribute("name")) {
                std::string name    = pr->Attribute("name");
                std::string id      = pr->Attribute("id");
                std::string cl      = pr->Attribute("class");

                Layout_element tmp(name, id, cl);
                View::layout_elements[name] = tmp;
                TiXmlNode* ch = node->FirstChild();

                for (int i = 0; i < count_childs(node); ++i) {
                    if (!ch->NoChildren()) {
                        //std::clog << ch->FirstChild()->Value() << std::endl;
                        this->layout_elements[name].set_content(ch->FirstChild()->Value());
                    }
                    ch = ch->NextSibling();
                }
            }
        }

        iterate(node);
    }
}

void View::load() {
    this->styles = parse("style.css");
    this->document.LoadFile("layout.xml");

    this->layout_elements.clear();

    View::iterate(this->document.RootElement());

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

            int t_color = t_style->get_t_color();
            int b_color = t_style->get_b_color();

            if (t_color >= 0) {
                int blue = t_color % 256;
                t_color /= 256;
                int red = t_color / 256;
                int green = t_color % 256;

                le.t_red = red;
                le.t_green = green;
                le.t_blue = blue;

                le.t_color_setted = true;
            }

            if (b_color >= 0) {
                int blue = b_color % 256;
                b_color /= 256;
                int red = b_color / 256;
                int green = b_color % 256;

                le.b_red = red;
                le.b_green = green;
                le.b_blue = blue;

                le.b_color_setted = true;
            }

            if (le.get_content().size() * drawer.get_fontsize() > t_style->get_height()) {
                t_style->set_height((le.get_content().size()-1) * drawer.get_fontsize() + 30);
            }

            std::vector <std::string> lines = le.get_content();
            std::string longest_line;
            std::vector <std::string> :: iterator longest;

            if (lines.size() == 0) {
                longest_line = "";
            } else if (lines.size() < 2) {
                longest_line = lines[0];
            } else {
                longest = std::max_element(lines.begin(), lines.end(), compare_length);
                longest_line = *longest;
            }

            if (longest_line.length() * (drawer.get_fontsize()/3) > t_style->get_width()) {
                t_style->set_width(longest_line.length() * (drawer.get_fontsize()/3)+40);
            }

            le.set_size(t_style->get_width(), t_style->get_height());
        }

        ++t_style;
    }
}

void View::draw() {
    std::map <std::string, Layout_element> :: iterator element;

    for (element = layout_elements.begin(); element != layout_elements.end(); ++element) {
        //std::clog << element->first << std::endl;
        style(element->second);
        draw_element(element->second);
    }
}

void View::add_element(std::string key, std::string id, std::string cl, std::string value) {
    Layout_element le(key, id, cl);
    this->layout_elements[key] = le;
    this->layout_elements[key].set_content(value);
}
