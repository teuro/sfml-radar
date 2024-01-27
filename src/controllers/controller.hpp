#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP

#include "../views/view.hpp"

class Controller {
public:
    virtual void handle_function_keys(int key) = 0;
    virtual void handle_mouse_click(Point& mouse) = 0;
    virtual void handle_mouse_release(Point& mouse_start, Point& mouse_end) = 0;
    virtual void handle_mouse_wheel(int amount) = 0;
    virtual void update(double elapsed) = 0;
    virtual void handle_text_input() = 0;
    virtual void update_command(std::string text) = 0;
	virtual void load() = 0;
	virtual void draw(Point& mouse) = 0;
protected:
    Controller(std::shared_ptr <Settings> s, Drawsurface& d);
    ~Controller();
	std::shared_ptr <Settings> settings;
	Drawsurface& drawer;
	std::string command;
	int fps;
	double frames;
	double fps_time;
	double fps_end_time;
};

#endif // CONTROLLER_HPP
