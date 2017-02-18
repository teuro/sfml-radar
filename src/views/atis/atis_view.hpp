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
    void load(std::vector <Runway> runways, std::map <int, std::vector <int> > levels);
	std::string get_value(Point& mouse);
	void draw_errors(std::list <std::string> errors);
	void update();
private:
	Atis* atis;
	std::vector <Drawable_list> lists;
	std::map <int, Drawable_list> display_levels;
	std::map <int, std::vector <int> > tr_levels;
	std::vector <Runway> runways;
};

#endif // ATIS_VIEW
