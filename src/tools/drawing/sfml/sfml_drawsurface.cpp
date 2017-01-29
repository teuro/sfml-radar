#include "sfml_drawsurface.hpp"

sfml_drawsurface::sfml_drawsurface(sf::RenderWindow& w) : window(w) {
	font_loaded = false;
}

sfml_drawsurface::~sfml_drawsurface() { }

void sfml_drawsurface::circleColor(Point& a, unsigned int radius, int red, int green, int blue) {
	sf::CircleShape circle(radius);
	circle.setOutlineColor(sf::Color(red, green, blue));
	circle.setOutlineThickness(2.0);
	circle.setPosition(sf::Vector2f(a.get_x()-radius, a.get_y()-radius));

	this->window.draw(circle);
}

void sfml_drawsurface::circleColor(Point& a, unsigned int radius, int color) {
	this->colors = this->convert(color);
	
	circleColor(a, radius, this->colors[0], this->colors[1], this->colors[2]);
}

void sfml_drawsurface::draw_picture(std::string file, Point& a) {
	sf::Texture image;
	sf::Sprite sprite;
	sf::Vector2f place(a.get_x(), a.get_y());
	
	image.loadFromFile(file);
	sprite.setTexture(image);
	
	sprite.setPosition(place);
	this->window.draw(sprite);
}

void sfml_drawsurface::draw_text(std::string text, Point& a, int red, int green, int blue, int font_size) {
	this->font_size = font_size;
	
	if (!font_loaded) {
		this->load_font("arial.ttf", this->font_size);
	}

	sf::Text _text;
	_text.setFont(this->font);
	_text.setCharacterSize(font_size);
	_text.setString(text);
	_text.setPosition(sf::Vector2f(a.get_x(), a.get_y()));

    sf::Color clr(red, green, blue);
	_text.setColor(clr);

	this->window.draw(_text);
}

void sfml_drawsurface::draw_text(std::string text, Point& a, int color, int font_size) {
	this->colors = this->convert(color);
	
	sfml_drawsurface::draw_text(text, a, this->colors[0], this->colors[1], this->colors[2], font_size);
}

void sfml_drawsurface::load_font(std::string _font, unsigned int font_size) {
	if (!this->font.loadFromFile("fonts/" + _font)) {
		std::logic_error("Requested font " + _font + " is not available");
	}

    this->font_size = font_size;

	font_loaded = true;
}

void sfml_drawsurface::flip() {
	this->window.display();
}

void sfml_drawsurface::lineColor(Point& a, Point& b, int color) {
	this->colors = this->convert(color);
	
	lineColor(a, b, this->colors[0], this->colors[1], this->colors[2]);
}

void sfml_drawsurface::lineColor(Point& a, Point& b, int red, int green, int blue) {
    sf::Color color(red, green, blue);

	sf::Vertex line[] = {
		sf::Vertex(sf::Vector2f(a.get_x(), a.get_y()), color),
		sf::Vertex(sf::Vector2f(b.get_x(), b.get_y()), color)
	};

	window.draw(line, 2, sf::Lines);
}

void sfml_drawsurface::rectangleColor(Point& a, Point& b, int red, int green, int blue, int border_color) {
	sf::RectangleShape rect(sf::Vector2f(b.get_x()-a.get_x(), b.get_y()-a.get_y()));

    sf::Color fclr(red, green, blue);
	
	int o_blue = border_color % 256;
	border_color /= 256;
	int o_red = border_color / 256;
	int o_green = border_color % 256;
	
    sf::Color oclr(o_red, o_green, o_blue);
	
    rect.setOutlineColor(oclr);
    rect.setOutlineThickness(5);
	rect.setFillColor(fclr);
	
	rect.setPosition(sf::Vector2f(a.get_x(), a.get_y()));

	this->window.draw(rect);
}

void sfml_drawsurface::rectangleColor(Point& a, Point& b, int color, int border_color) {
	this->colors = this->convert(color);
	
	sfml_drawsurface::rectangleColor(a, b, this->colors[0], this->colors[1], this->colors[2], border_color);
}

void sfml_drawsurface::rectangleColor(Point& a, unsigned int length, int red, int green, int blue, int border_color) {
	Point b(a.get_x() + length, a.get_y() + length);
	sfml_drawsurface::rectangleColor(a, b, red, green, blue, border_color);
}

void sfml_drawsurface::trigonColor(Point& a, unsigned int size, int color) {
	this->colors = this->convert(color);
	
	trigonColor(a, size, this->colors[0], this->colors[1], this->colors[2]);
}

void sfml_drawsurface::trigonColor(Point& a, unsigned int size, int red, int green, int blue) {
	sf::CircleShape triangle(size, 3);
	triangle.setOutlineColor(sf::Color(red, green, blue));
	triangle.setPosition(a.get_x(), a.get_y());
	
	this->window.draw(triangle);
}

unsigned int sfml_drawsurface::get_fontsize() {
	return this->font_size;
}

void sfml_drawsurface::clear_screen() {
	this->window.clear();
}

std::vector <int> sfml_drawsurface::convert(int color) {
	std::vector <int> t;
	int blue = color % 256;
	color /= 256;
	int red = color / 256;
	int green = color % 256;
	
	t.push_back(red);
	t.push_back(green);
	t.push_back(blue);
	
	return t;
}
