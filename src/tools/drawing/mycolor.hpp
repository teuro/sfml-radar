#ifndef MY_COLOR_HPP
#define MY_COLOR_HPP

class My_Color {
public:
	int red;
	int green;
	int blue;
	
	My_Color(int red, int green, int blue);
	explicit My_Color(int color);
	
	int get_color();
	
	bool operator ==(const My_Color& b);
private:
	int color;
	
	void calculate();
};

#endif