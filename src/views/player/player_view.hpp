#ifndef PLAYER_VIEW
#define PLAYER_VIEW

#include <vector>
#include <list>
#include <iostream>

#include "../view.hpp"
#include "../../tools/tools.hpp"
#include "../../tools/settings.hpp"
#include "../../models/player.hpp"

class Playerview : public View {
public:
	Playerview(Drawsurface& d, Settings& s);
	~Playerview();
	void draw();
private:
    Settings& settings;
};

#endif // PLAYER_VIEW
