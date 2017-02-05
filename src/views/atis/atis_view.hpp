#ifndef ATIS_VIEW
#define ATIS_VIEW

#include <vector>
#include <iostream>

#include "../view.hpp"
#include "../../tools/tools.hpp"
#include "../../tools/settings.hpp"
#include "../../models/atis.hpp"
#include "../../models/runway.hpp"

class Atisview : public View {
public:
	Atisview(Drawsurface& d, Settings& s, Atis*& a);
	~Atisview();
	void draw();
    void load(std::vector <Runway> runways);
	std::string get_value(Point& mouse);
private:
	Atis* atis;
	std::vector <Runway> runways;
};

#endif // ATIS_VIEW
