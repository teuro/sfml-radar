#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP

#include "../views/view.hpp"

class Controller {
public:
    virtual std::string handle_function_keys(int key) = 0;
    virtual void handle_mouse_click(Point& mouse) = 0;
    virtual void handle_mouse_release(Point& mouse_start, Point& mouse_end) = 0;
    virtual void handle_mouse_wheel(int amount) = 0;
    virtual void update(double elapsed, bool draw) = 0;
    virtual void handle_text_input() = 0;
    virtual void update_command(std::string text) = 0;
    virtual bool is_ok() = 0;
	virtual void load() = 0;
	//virtual std::string get_input() = 0;
protected:
    Controller(Settings& s, Drawsurface& d) : settings(s), drawer(d) { }
    virtual ~Controller() { }
	Settings& settings;
	Drawsurface& drawer;
	std::string command;
	int fps;
	double frames;
private:
};


#endif // CONTROLLER_HPP
