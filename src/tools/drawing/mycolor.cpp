#include "mycolor.hpp"

My_Color::My_Color(int r, int g, int b) : red(r), green(g), blue(b) { 
	this->color = (this->red * 256 + this->green) * 256 + this->blue;
}

My_Color::My_Color(int c) : color(c) { 
	this->calculate(); 
}

void My_Color::calculate() {
	this->blue = this->color % 256;
	this->color /= 256;
	this->red = this->color / 256;
	this->green = this->color % 256;
}

int My_Color::get_color() {
	return this->color;
}