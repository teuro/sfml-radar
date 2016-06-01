#include "player_view.hpp"

Playerview::Playerview(Drawsurface& d, Settings& s) : View(d, s) { }

Playerview::~Playerview() { }

void Playerview::draw() {
    Point center_point(this->settings.screen_width / 2, this->settings.screen_height/2);
    drawer.clear_screen();

    drawer.draw_text("Playerview", center_point, "red");

    drawer.flip();
}
