#ifndef STAT_VIEW
#define STAT_VIEW

#include <vector>
#include <iostream>

#include "../view.hpp"
#include <tools/tools.hpp>
#include "../../tools/point_tools.hpp"
#include <tools/time_tools.hpp>
#include "../../tools/settings.hpp"

class Statview : public View {
public:
	Statview(Drawsurface& drawsurface, std::shared_ptr <Settings> settings);
	~Statview();
	virtual void draw(Point& mouse) override;
	virtual std::string handle_click(Point& mouse) override;
    void load() override;
	void draw_points();
	virtual void update() override;
	void set_handled_planes(Queryresult handled_planes);
private:
	bool loaded;
	Queryresult handled_planes;
};

#endif // ATIS_VIEW
