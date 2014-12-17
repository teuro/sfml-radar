#ifndef ATIS_VIEW
#define ATIS_VIEW

#include <vector>
#include <list>
#include <iostream>

#include "view.hpp"
#include "../tools/tools.hpp"
#include "../tools/settings.hpp"
#include "../models/atis.hpp"

class Atisview : public View {
public:
	Atisview(Atis& m, Drawsurface& d, Settings& s);
	~Atisview();
	void draw();
private:
    Atis& atis;
    Settings& settings;
};

#endif // ATIS_VIEW
