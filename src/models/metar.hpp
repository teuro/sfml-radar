#ifndef METAR_HPP
#define METAR_HPP

#include <list>
#include <string>

#include "../tools/tools.hpp"
#include "../tools/database/database.hpp"

/**
	* Cloud represent abstract base of clouds
	* @var type
	* @var	altitude
**/

struct Cloud {
    std::string type;
    int altitude;
};

/**
	* \struct Wind represent abstract base of winds
**/

struct Wind {
	/** \var direction **/
    int direction;
	
	/** \var speed **/
    int speed;
};

class Metar {
public:
    Metar();
    ~Metar();
    std::string to_string();
    void update(std::string icao);
    int get_pressure();
    double get_wind_direction();
    int get_wind_speed();
    int get_temperature();
    int get_visibility();
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
	void generate_clouds();
};

#endif // METAR_HPP
