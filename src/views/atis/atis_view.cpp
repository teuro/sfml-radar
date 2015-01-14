#include "atis_view.hpp"

Atisview::Atisview(Drawsurface& d, Settings& s) : View(d), settings(s) {
    this->load();
}

Atisview::~Atisview() { }

void Atisview::load() {
    View::load();
}

void Atisview::draw(std::string dep, std::string lnd, int tr_alt, int tr_lvl) {
    this->load();
    Point place(10, 60);

    View::draw();

    this->drawer.draw_text(Tools::trim(dep), place, "red");
    place.change_y(drawer.get_fontsize());
    this->drawer.draw_text(Tools::trim(lnd), place, "red");
    place.change_y(drawer.get_fontsize());
    this->drawer.draw_text(Tools::tostr(tr_alt), place, "red");
    place.change_y(drawer.get_fontsize());
    this->drawer.draw_text(Tools::tostr(tr_lvl), place, "red");
}
