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

#include "drawsurface.hpp"

#include <stdexcept>
#include <SFML/Graphics.hpp>

class sfml_drawsurface : public Drawsurface {
public:
	sfml_drawsurface(sf::RenderWindow& window);
	~sfml_drawsurface();

	void rectangleColor(Point& a, Point& b, std::string color, bool border = false);
	void rectangleColor(Point& a, unsigned int length, std::string color);
    void circleColor(Point& a, unsigned int rad, std::string color);
    void lineColor(Point& a, Point& b, std::string color);
    void trigonColor(Point& a, Point& b, Point& c, std::string color);
    void trigonColor(Point& a, unsigned int _size, std::string color);

    void draw_picture(std::string tiedosto, Point& a, bool center = false);
    void draw_text(std::string text, Point& a, std::string color = " black", int font_size = 16);
    void load_font(std::string font, unsigned int font_size);
    void clear_screen();

	int get_fontsize();
	void flip();
protected:
private:
	sf::RenderWindow& window;
	sf::Font font;

    sf::Color active;
    sf::Color normal;
    sf::Color failure;

    std::map <std::string, sf::Color> colors;
};

#endif // SFML_DRAWSURFACE_HPP
