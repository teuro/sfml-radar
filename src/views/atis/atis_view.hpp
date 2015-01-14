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
	void draw(std::string dep, std::string lnd, int tr_alt, int tr_lvl);
private:
    Settings& settings;
    void load();
};

#endif // ATIS_VIEW
