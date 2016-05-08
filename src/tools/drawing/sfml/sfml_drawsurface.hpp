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

	void rectangleColor(Point& a, Point& b, std::string color, bool border);
    void rectangleColor(Point& a, Point& b, int red, int green, int blue, bool border);
    void rectangleColor(Point& a, unsigned int lenght, std::string color);
    void rectangleColor(Point& a, unsigned int length, int red, int green, int blue, bool border);

    void lineColor(Point& a, Point& b, std::string color);
    void lineColor(Point& a, Point& b, int red, int green, int blue);

    void circleColor(Point& a, unsigned int rad, std::string color);
    void circleColor(Point& a, unsigned int rad, int red, int green, int blue);

    void trigonColor(Point& a, Point& b, Point& c, std::string color);
    void trigonColor(Point& a, Point& b, Point& c, int red, int green, int blue);
    void trigonColor(Point& a, unsigned int size, std::string color);
    void trigonColor(Point& a, unsigned int size, int red, int green, int blue);

    void load_font(std::string font, unsigned int font_size);

    void draw_picture(std::string tiedosto, Point& a, bool center = false);

    void draw_text(std::string text, Point& a, std::string color = "green", int font_size = 16);
    void draw_text(std::string text, Point& a, int red, int green, int blue, int font_size = 16);

	unsigned int get_fontsize();
	void flip();
	void clear_screen();
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
