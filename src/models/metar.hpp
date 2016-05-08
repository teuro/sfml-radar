#ifndef METAR_HPP
#define METAR_HPP

#include <list>
#include <string>

#include "../tools/tools.hpp"
#include "database.hpp"

struct Cloud {
    std::string type;
    int altitude;
};

struct Wind {
    int direction;
    int speed;
};

class Metar {
public:
    Metar();
    ~Metar();
    std::string get_metar();
    void update(std::string icao);
    int get_pressure();
    int get_wind_direction();
    int get_temperature();
    int get_visibility();
    int get_correct_level(int transition_altitude);
protected:
private:
    int pressure;
    int humidity;
    int temperature;
    int devpoint;
    int visibility;
    std::list <Cloud> clouds;
    Wind wind;
    std::string icao;
};

#endif // METAR_HPP
