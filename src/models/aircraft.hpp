#ifndef _AIRCRAFT_HPP
#define _AIRCRAFT_HPP

#include "coordinate.hpp"
#include "../tools/tools.hpp"
#include "clearance.hpp"

#include <stack>

class Aircraft {
public:
    Aircraft(std::string name, int speed, int heading, int altitude, Coordinate p, int type);
    ~Aircraft();
    void update(double elapsed);

    Coordinate& get_place();
    std::string get_name();
    void set_clearance(Clearance& cl);

    double get_speed();
    double get_heading();
    double get_altitude();
    int get_turn();

    double get_clearance_speed();
    double get_clearance_heading();
    double get_clearance_altitude();

    void set_separation_error(bool t);
    bool get_separation_error();
    int get_type();
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
    int type;
};


#endif
