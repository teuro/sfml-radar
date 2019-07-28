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
	
	void rectangleColor(Point& a, Point& b, int color, int border_color = 0) override;
    void rectangleColor(Point& a, Point& b, int red, int green, int blue, int border_color = 0) override;
    void rectangleColor(Point& a, unsigned int length, int red, int green, int blue, int border_color = 0) override;

    void lineColor(Point& a, Point& b, int color) override;
    void lineColor(Point& a, Point& b, int red, int green, int blue) override;

    void circleColor(Point& a, unsigned int radius, int color, bool debug = false) override;
    void circleColor(Point& a, unsigned int rad, int red, int green, int blue, bool debug = false) override;

    void trigonColor(Point& a, unsigned int size, int color) override;
    void trigonColor(Point& a, unsigned int size, int red, int green, int blue) override;

    void load_font(std::string font, unsigned int font_size) override;

    void draw_picture(std::string tiedosto, Point& a) override;

    void draw_text(std::string text, Point& a, std::string color = "green", int font_size = 16);
    void draw_text(std::string text, Point& a, int red, int green, int blue, int font_size = 16) override;
    void draw_text(std::string text, Point& a, int color, int font_size = 16) override;
	
	int get_text_length(std::string text, int font_size) override;
	int get_text_height(std::string text, int font_size) override;

	void flip() override;
	void clear_screen(int red, int green, int blue) override;
	void clear_screen(int color) override;
protected:
private:
	sf::RenderWindow& window;
	sf::Font font;
};

#endif // SFML_DRAWSURFACE_HPP
