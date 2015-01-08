#ifndef ATISCONTROLLER_HPP
#define ATISCONTROLLER_HPP

#include "../models/atis.hpp"
#include "../views/atis/atis_view.hpp"
#include "controller.hpp"

class Atiscontroller : public Controller {
public:
    Atiscontroller(Atisview& av, Settings& s, Atis& a);
    ~Atiscontroller();
    void handle_mouse_click(Point& mouse);
    void handle_mouse_release(Point& mouse_start, Point& mouse_end);
    void handle_mouse_wheel(int amount);
    void update(double elapsed, bool draw);
    void handle_function_keys(int key);
    void handle_text_input();
    void update_command(std::string command);
protected:
private:
    Atisview& atisview;
    Atis& atis;
};


#endif // ATISCONTROLLER_HPP
