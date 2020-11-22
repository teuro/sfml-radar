#ifndef SFML_DRAWSURFACE_HPP
#define SFML_DRAWSURFACE_HPP
/**
    * sfml-drawsurface provides basic drawing, pictures and text
    * @date 2014/06/02
    * @author Juha Teurokoski
**/

#include "../drawsurface.hpp"

#include <stdexcept>
#include <SFML/Graphics.hpp>

class sfml_drawsurface : public Drawsurface {
public:
	explicit sfml_drawsurface(sf::RenderWindow& window);
	~sfml_drawsurface();
	
	virtual void rectangleColor(Point& a, Point& b, unsigned int color, unsigned int border_color);

    virtual void lineColor(Point& a, Point& b, unsigned int color);
    
    virtual void circleColor(Point& a, unsigned int radius, unsigned int color);
    
    virtual void trigonColor(Point& a, unsigned int size, unsigned int color, unsigned int border_color);

    virtual void load_font(std::string font);

    virtual void draw_picture(std::string tiedosto, Point& a);
	
    virtual void draw_text(std::string text, Point& a, unsigned int color, int font_size = 16);
	
	virtual int get_text_length(std::string text, int font_size);
	virtual int get_text_height(std::string text, int font_size);

	virtual void flip();
	virtual void clear_screen(unsigned int color);
protected:
private:
	sf::RenderWindow& window;
	sf::Font font;
};

#endif // SFML_DRAWSURFACE_HPP
