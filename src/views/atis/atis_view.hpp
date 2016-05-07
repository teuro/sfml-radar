#ifndef ATIS_VIEW
#define ATIS_VIEW

#include <vector>
#include <list>
#include <iostream>

#include "../view.hpp"
#include "../../tools/tools.hpp"
#include "../../tools/settings.hpp"
#include "../../models/atis.hpp"

class Atisview : public View {
public:
	Atisview(Drawsurface& d, Settings& s);
	~Atisview();
	void draw();
	void draw_runways(std::list <std::string>& runways);
private:
    Settings& settings;
    void load();
};

#endif // ATIS_VIEW
