#ifndef _VIEW_HPP
#define _VIEW_HPP

#include <iostream>

#include "../tools/drawing/sfml/sfml_drawsurface.hpp"
#include <map>

class View {
public:
	View(Drawsurface& d) : drawer(d) { }
	virtual void draw() = 0;
protected:
    Drawsurface& drawer;
};


#endif
