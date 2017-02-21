/**
    * drawsurface base for all graphics pure abstract. Provide only interface quite high-level
    * @date 2014/06/02
    * @author Juha Teurokoski
**/

#ifndef _DRAWSURFACE_HPP
#define _DRAWSURFACE_HPP

#include <string>
#include <vector>

#include "../../models/point.hpp"

class Drawsurface {
protected:
	std::vector <int> colors;
	virtual std::vector <int> convert(int color) = 0;
public:
	bool font_loaded;
	unsigned int font_size;
		
    virtual void rectangleColor(Point& a, Point& b, int color, int border_color = 0) = 0;
    virtual void rectangleColor(Point& a, Point& b, int red, int green, int blue, int border_color = 0) = 0;
    virtual void rectangleColor(Point& a, unsigned int length, int red, int green, int blue, int border_color = 0) = 0;

    virtual void lineColor(Point& a, Point& b, int red, int green, int blue) = 0;
    virtual void lineColor(Point& a, Point& b, int color) = 0;

    virtual void circleColor(Point& a, unsigned int radius, int red, int green, int blue) = 0;
    virtual void circleColor(Point& a, unsigned int radius, int color) = 0;

    virtual void trigonColor(Point& a, unsigned int size, int color) = 0;
    virtual void trigonColor(Point& a, unsigned int size, int red, int green, int blue) = 0;

    virtual void load_font(std::string font, unsigned int font_size) = 0;

    virtual void draw_picture(std::string tiedosto, Point& a) = 0;
	
    virtual void draw_text(std::string text, Point& a, int red, int green, int blue, int font_size = 16) = 0;
    virtual void draw_text(std::string text, Point& a, int color, int font_size = 16) = 0;
	
	virtual int get_text_length(std::string text) = 0;

	virtual unsigned int get_fontsize() = 0;
	virtual void flip() = 0;
	virtual void clear_screen() = 0;

    virtual ~Drawsurface() { }
};

#endif
