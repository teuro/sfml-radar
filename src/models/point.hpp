/**
    * point responsible low-level point of screen
    * MVC-
    * 2014/06/02
    * Juha Teurokoski
**/

#ifndef _POINT_HPP
#define _POINT_HPP

#include <iostream>

class Point {
public:
	Point() { }
	Point(int x, int y);
	~Point();
	int get_x();
	int get_y();

	void set_x(int x);
	void set_y(int y);

	void set_place(int x, int y);

	void change_y(int amount);
	void change_x(int amount);

	friend std::ostream& operator << (std::ostream& virta, const Point& olio);
protected:
private:
	int x;
	int y;
};


#endif // _POINT_HPP
