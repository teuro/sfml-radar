#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP

#include "../views/view.hpp"

class Controller {
public:
    virtual void handle_function_keys(int key) = 0;
    virtual void handle_mouse_click(Point& mouse) = 0;
    virtual void handle_mouse_release(Point& mouse_start, Point& mouse_end) = 0;
    virtual void handle_mouse_wheel(int amount) = 0;
    virtual void update(double elapsed, bool draw) {
        this->update(elapsed);
        if (draw) {
            this->view->draw();
        }
    }
    virtual void update(double elapsed) = 0;
    virtual void handle_text_input(std::string text) = 0;
protected:
    Controller(View* v, Settings& s) : view(v), settings(s) { }
    ~Controller() { }
    View* view;
    Settings& settings;
private:
};


#endif // CONTROLLER_HPP
