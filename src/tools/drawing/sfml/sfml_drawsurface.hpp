#ifndef SFML_DRAWSURFACE_HPP
#define SFML_DRAWSURFACE_HPP
/**
    * sfml-drawsurface provides basic drawing, pictures and text
    * @require drawsurface
    * @require stdexcept
    * @require SFML\graphics
    * 2014/06/02
    * Juha Teurokoski
**/

#include "../drawsurface.hpp"

#include <stdexcept>
#include <SFML/Graphics.hpp>

class sfml_drawsurface : public Drawsurface {
public:
	sfml_drawsurface(sf::RenderWindow& window);
	~sfml_drawsurface();
	
	std::vector <int> convert (int color);
	
	void rectangleColor(Point& a, Point& b, int color, int border_color = 0);
    void rectangleColor(Point& a, Point& b, int red, int green, int blue, int border_color = 0);
    void rectangleColor(Point& a, unsigned int length, int red, int green, int blue, int border_color = 0);

    void lineColor(Point& a, Point& b, int color);
    void lineColor(Point& a, Point& b, int red, int green, int blue);

    void circleColor(Point& a, unsigned int radius, int color);
    void circleColor(Point& a, unsigned int rad, int red, int green, int blue);

    void trigonColor(Point& a, unsigned int size, int color);
    void trigonColor(Point& a, unsigned int size, int red, int green, int blue);

    void load_font(std::string font, unsigned int font_size);

    void draw_picture(std::string tiedosto, Point& a);

    void draw_text(std::string text, Point& a, std::string color = "green", int font_size = 16);
    void draw_text(std::string text, Point& a, int red, int green, int blue, int font_size = 16);
    void draw_text(std::string text, Point& a, int color, int font_size = 16);

	unsigned int get_fontsize();
	void flip();
	void clear_screen();
protected:
private:
	sf::RenderWindow& window;
	sf::Font font;
};

#endif // SFML_DRAWSURFACE_HPP
