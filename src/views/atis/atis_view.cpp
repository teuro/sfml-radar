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
    View::draw();
}
