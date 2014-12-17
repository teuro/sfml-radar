#ifndef _AIRCRAFT_HPP
#define _AIRCRAFT_HPP

#include "coordinate.hpp"
#include "../tools/tools.hpp"
#include "clearance.hpp"

#include <stack>

class Aircraft {
public:
    Aircraft(std::string name, int speed, int heading, int altitude, Coordinate p);
    ~Aircraft();
    void update(double elapsed);

    Coordinate& get_place();
    std::string get_name();
    void set_clearance(Clearance& cl);

    double get_speed() { return this->speed; }
    double get_heading() { return this->heading; }
    double get_altitude() { return this->altitude; }

    double get_clearance_speed() { return this->clearance_speed; }
    double get_clearance_heading() { return this->clearance_heading; }
    double get_clearance_altitude() { return this->clearance_altitude; }

    void set_separation_error(bool t) { this->separation_error = t; }
    bool get_separation_error() { return this->separation_error; }
protected:
private:
    Coordinate place;
    std::string name;
    bool separation_error;

    double speed;
    double heading;
    double altitude;

    double clearance_speed;
    double clearance_altitude;
    double clearance_heading;
    int turn;

    std::stack <Clearance> clearances;

    void handle_clearance(Clearance& ac);
    void change_speed();
    void change_altitude(double elapsed);
    void change_heading();
    double change_parameter(double elapsed, double value, double _value, double change, int turn = 0);
};


#endif
