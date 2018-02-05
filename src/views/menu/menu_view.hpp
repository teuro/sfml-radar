#ifndef MENU_VIEW
#define MENU_VIEW

#include <vector>
#include <iostream>

#include "../view.hpp"
#include "../../models/menu/menu.hpp"
#include "../../tools/tools.hpp"
#include "../../tools/point_tools.hpp"
#include "../../tools/settings.hpp"

class Menuview : public View {
public:
	Menuview(Drawsurface& d, std::shared_ptr <Settings> s, std::shared_ptr <Menu> menu);
	~Menuview();
	virtual void draw(Point& mouse);
	virtual std::string handle_click(Point& mouse);
    void load();
	void set_menu(std::shared_ptr <Menu> menu);
	virtual void update();
private: 
	std::shared_ptr <Menu> menu;
};

#endif // ATIS_VIEW
