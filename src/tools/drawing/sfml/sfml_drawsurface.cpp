#include "sfml_drawsurface.hpp"

sfml_drawsurface::sfml_drawsurface(sf::RenderWindow& w) : window(w) {
	this->font_loaded = false;
}

sfml_drawsurface::~sfml_drawsurface() { }

void sfml_drawsurface::circleColor(Point& a, unsigned int radius, unsigned int color) {
	sf::CircleShape circle(radius);
	
	My_Color t_color(color);
	
	circle.setOutlineColor(sf::Color(t_color.red, t_color.green, t_color.blue));
	circle.setOutlineThickness(2.0);
	
	circle.setPosition(sf::Vector2f(a.get_x()-radius, a.get_y()-radius));

	this->window.draw(circle);
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

void sfml_drawsurface::draw_text(std::string text, Point& a, unsigned int color, int font_size) {
	if (!font_loaded) {
		this->load_font("arial.ttf");
	}
	
	My_Color t_color(color);
	sf::Color clr(t_color.red, t_color.green, t_color.blue);
	
	sf::Text _text;
	
	_text.setFont(this->font);
	_text.setCharacterSize(font_size);
	_text.setPosition(sf::Vector2f(a.get_x(), a.get_y()));
	_text.setFillColor(clr);
	_text.setString(text);

	this->window.draw(_text);
}

void sfml_drawsurface::load_font(std::string _font) {
	if (!this->font.loadFromFile("fonts/" + _font)) {
		std::logic_error("Requested font " + _font + " is not available");
	}

	font_loaded = true;
}

void sfml_drawsurface::flip() {
	this->window.display();
}

void sfml_drawsurface::lineColor(Point& a, Point& b, unsigned int color) {
	
	My_Color t_color(color);
	sf::Color clr(t_color.red, t_color.green, t_color.blue);
	
	sf::Vertex line[] = {
		sf::Vertex(sf::Vector2f(a.get_x(), a.get_y()), clr),
		sf::Vertex(sf::Vector2f(b.get_x(), b.get_y()), clr)
	};

	window.draw(line, 2, sf::Lines);
}

void sfml_drawsurface::rectangleColor(Point& a, Point& b, unsigned int color,  unsigned int border_color) {
	sf::RectangleShape rect(sf::Vector2f(b.get_x()-a.get_x(), b.get_y()-a.get_y()));

    My_Color fclr(color);
    My_Color oclr(border_color);
	
    rect.setOutlineColor(sf::Color(oclr.red, oclr.green, oclr.blue));
    rect.setOutlineThickness(5);
	rect.setFillColor(sf::Color(fclr.red, fclr.green, fclr.blue));
	
	rect.setPosition(sf::Vector2f(a.get_x(), a.get_y()));

	this->window.draw(rect);
}

void sfml_drawsurface::trigonColor(Point& a, unsigned int size, unsigned int color,  unsigned int border_color) {
	sf::CircleShape triangle(size, 3);
	
	My_Color text_color(color);
	My_Color b_color(border_color);
	
	triangle.setOutlineColor(sf::Color(b_color.red, b_color.green, b_color.blue));
	triangle.setFillColor(sf::Color(text_color.red, text_color.green, text_color.blue));
	
	triangle.setPosition(a.get_x() - size, a.get_y() - size);
	
	this->window.draw(triangle);
}

void sfml_drawsurface::clear_screen(unsigned int color) {
	//std::clog << "sfml_drawsurface::clear_screen(" << red << ", " << green << ", " << blue << ")" << std::endl;
	My_Color t_color(color);
	sf::Color background(t_color.red, t_color.green, t_color.blue, 0);
	this->window.clear(background);
}

int sfml_drawsurface::get_text_length(std::string text, int font_size) {
	if (!font_loaded) {
		this->load_font("arial.ttf");
	}
	
	sf::Text t_text;
	t_text.setFont(this->font);
	t_text.setString(text);
	t_text.setCharacterSize(font_size);
	sf::FloatRect t_rect = t_text.getLocalBounds();
	
	return (int)t_rect.width;
}

int sfml_drawsurface::get_text_height(std::string text, int font_size) {
	if (!font_loaded) {
		this->load_font("arial.ttf");
	}
	
	sf::Text t_text;
	t_text.setFont(this->font);
	t_text.setString(text);
	t_text.setCharacterSize(font_size);
	sf::FloatRect t_rect = t_text.getLocalBounds();
	return (int)t_rect.height;
}
