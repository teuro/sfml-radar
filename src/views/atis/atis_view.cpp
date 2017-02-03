#include "atis_view.hpp"

Rwy::Rwy(std::string n, Point p1, Point p2) : name(n), place_lu(p1), place_rd(p2) { }

Atisview::Atisview(Drawsurface& d, Settings& s) : View(d, s) {
    this->load();
}

Atisview::~Atisview() { }

void Atisview::load() {
    View::load("atis");
}

void Atisview::draw() {
    View::draw();
}

