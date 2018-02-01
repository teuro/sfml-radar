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
	Menuview(Drawsurface& d, std::shared_ptr <Settings> s, Menu*& m);
	~Menuview();
	void draw();
    void load();
	void set_menu(Menu*& menu);
private: 
	Menu* menu;
};

#endif // ATIS_VIEW
