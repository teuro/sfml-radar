/**
    * drawsurface base for all graphics pure abstract. Provide only interface quite high-level
    * @date 2014/06/02
    * @author Juha Teurokoski
**/

#ifndef _DRAWSURFACE_HPP
#define _DRAWSURFACE_HPP

#include <string>
#include <vector>
#include <map>

#include "../../models/coordinate/point.hpp"
#include "mycolor.hpp"

class Drawsurface {
protected:
	
public:
	bool font_loaded;
		
    virtual void rectangleColor(Point& a, Point& b, unsigned int color, unsigned int border_color) = 0;

    virtual void lineColor(Point& a, Point& b, unsigned int color) = 0;
    
    virtual void circleColor(Point& a, unsigned int radius, unsigned int color) = 0;
    
    virtual void trigonColor(Point& a, unsigned int size, unsigned int color, unsigned int border_color) = 0;

    virtual void load_font(std::string font) = 0;

    virtual void draw_picture(std::string tiedosto, Point& a) = 0;
	
    virtual void draw_text(std::string text, Point& a, unsigned int color, int font_size = 16) = 0;
	
	virtual int get_text_length(std::string text, int font_size) = 0;
	virtual int get_text_height(std::string text, int font_size) = 0;

	virtual void flip() = 0;
	virtual void clear_screen(unsigned int color) = 0;

    virtual ~Drawsurface() { }
};

#endif
