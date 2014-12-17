#include "atis_view.hpp"

Atisview::Atisview(Atis& a, Drawsurface& d, Settings& s) : View(d), atis(a), settings(s) { }

Atisview::~Atisview() { }

void Atisview::draw() {
    Point center_point(this->settings.screen_width / 2, this->settings.screen_height/2);
    drawer.clear_screen();

    drawer.draw_text("Atisview", center_point, "red");

    drawer.flip();
}
