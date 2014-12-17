#ifndef _VIEW_HPP
#define _VIEW_HPP

#include <iostream>

#include "../tools/sfml_drawsurface.hpp"
#include <map>

class View {
public:
	View(Drawsurface& d) : drawer(d) { }
	virtual void draw() = 0;
protected:
    Drawsurface& drawer;
};


#endif
