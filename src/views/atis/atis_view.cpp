#include "atis_view.hpp"

Atisview::Atisview(Drawsurface& d, Settings& s) : View(d), settings(s) {
    this->load();
}

Atisview::~Atisview() { }

void Atisview::load() {
    View::load();

    TinyXPath::xpath_processor element(this->document.RootElement(), "//element");
    //TinyXPath::xpath_processor metar(this->document.RootElement(), "//metar");

    unsigned int elements_size = element.u_compute_xpath_node_set();

    for (unsigned int i = 0; i < elements_size; ++i) {
        TiXmlElement* t = element.XNp_get_xpath_node(i)->ToElement();

        std::string name = element.XNp_get_xpath_node(i)->FirstChild()->FirstChild()->Value();
        if (t->Attribute("id") && t->Attribute("class")) {
            std::string id = t->Attribute("id");
            std::string cl = t->Attribute("class");

            Layout_element tmp(name, id, cl);
            View::layout_elements[name] = tmp;
        }
    }
}

void Atisview::draw(std::string dep, std::string lnd, int tr_alt, int tr_lvl) {
    Point place(10, 10);

    View::draw();

    this->drawer.draw_text(Tools::trim(dep), place, "red");
    place.change_y(drawer.get_fontsize());
    this->drawer.draw_text(Tools::trim(lnd), place, "red");
    place.change_y(drawer.get_fontsize());
    this->drawer.draw_text(Tools::tostr(tr_alt), place, "red");
    place.change_y(drawer.get_fontsize());
    this->drawer.draw_text(Tools::tostr(tr_lvl), place, "red");
}

void Atisview::update_command(std::string command) {
    View::layout_elements["Input"].update_content(command);
}
