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
	this->convert(color);
	
	circleColor(a, radius, this->red, this->green, this->blue);
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
	this->convert(color);
	
	sfml_drawsurface::draw_text(text, a, red, green, blue, font_size);
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
	this->convert(color);
	
	lineColor(a, b, this->red, this->green, this->blue);
}

void sfml_drawsurface::lineColor(Point& a, Point& b, int red, int green, int blue) {
    sf::Color color(red, green, blue);

	sf::Vertex line[] = {
		sf::Vertex(sf::Vector2f(a.get_x(), a.get_y()), color),
		sf::Vertex(sf::Vector2f(b.get_x(), b.get_y()), color)
	};

	window.draw(line, 2, sf::Lines);
}

void sfml_drawsurface::rectangleColor(Point& a, Point& b, int red, int green, int blue) {
	sf::RectangleShape rect(sf::Vector2f(b.get_x()-a.get_x(), b.get_y()-a.get_y()));

    sf::Color fclr(red, green, blue);
    rect.setFillColor(fclr);

	rect.setPosition(sf::Vector2f(a.get_x(), a.get_y()));

	this->window.draw(rect);
}

void sfml_drawsurface::rectangleColor(Point& a, Point& b, int color) {
	this->convert(color);
	
	sfml_drawsurface::rectangleColor(a, b, red, green, blue);
}

void sfml_drawsurface::rectangleColor(Point& a, unsigned int length, int red, int green, int blue) {
    sf::Color color(red, green, blue);

	sf::RectangleShape rect(sf::Vector2f(length, length));
	rect.setOutlineColor(color);

	rect.setPosition(sf::Vector2f(a.get_x()-length/2.0, a.get_y()-length/2.0));

	this->window.draw(rect);
}


void sfml_drawsurface::trigonColor(Point& a, Point& b, Point& c) {
	sf::VertexArray triangle(sf::Triangles, 3);

	// define the position of the triangle's points
	triangle[0].position = sf::Vector2f(a.get_x(), a.get_y());
	triangle[1].position = sf::Vector2f(b.get_x(), b.get_y());
	triangle[2].position = sf::Vector2f(c.get_x(), c.get_y());

	window.draw(triangle);
}

void sfml_drawsurface::trigonColor(Point& a, Point& b, Point& c, int red, int green, int blue) {
    sf::Color color(red, green, blue);

	sf::VertexArray triangle(sf::Triangles, 3);

	// define the position of the triangle's points
	triangle[0].position = sf::Vector2f(a.get_x(), a.get_y());
	triangle[1].position = sf::Vector2f(b.get_x(), b.get_y());
	triangle[2].position = sf::Vector2f(c.get_x(), c.get_y());

	window.draw(triangle);
}

void sfml_drawsurface::trigonColor(Point& a, unsigned int size) {
	Point a1(a.get_x(), a.get_y() - size);
	Point a2(a.get_x() - size, a.get_y() + size);
	Point a3(a.get_x() + size, a.get_y() + size);

	trigonColor(a1, a2, a3);
}

void sfml_drawsurface::trigonColor(Point& a, unsigned int size, int red, int green, int blue) {
	Point a1(a.get_x(), a.get_y() - size);
	Point a2(a.get_x() - size, a.get_y() + size);
	Point a3(a.get_x() + size, a.get_y() + size);

	trigonColor(a1, a2, a3, red, green, blue);
}

unsigned int sfml_drawsurface::get_fontsize() {
	return this->font_size;
}

void sfml_drawsurface::clear_screen() {
	this->window.clear();
}

void sfml_drawsurface::convert(int color) {
	blue = color % 256;
	color /= 256;
	red = color / 256;
	green = color % 256;
}
