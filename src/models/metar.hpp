#ifndef METAR_HPP
#define METAR_HPP

#include <list>
#include <string>

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
    void update();
protected:
private:
    int pressure;
    int humidity;
    int temperature;
    int visibility;
    std::list <Cloud> clouds;
    Wind wind;
};

#endif // METAR_HPP
