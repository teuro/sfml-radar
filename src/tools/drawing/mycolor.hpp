#ifndef MY_COLOR_HPP
#define MY_COLOR_HPP

class My_Color {
public:
	int red;
	int green;
	int blue;
	
	My_Color(int r, int g, int b);
	My_Color(int c);
	
	int get_color();
private:
	int color;
	
	void calculate();
};

#endif