#ifndef METAR_HPP
#define METAR_HPP

#include <list>
#include <string>
#include <ctime>
#include <map>
#include <random>
#include <memory>

#include "../../tools/tools.hpp"
#include "../../tools/settings.hpp"
#include "../../tools/text_tools.hpp"
#include "../../tools/database/database.hpp"

/**
	* Cloud represent abstract base of clouds
	* @var type
	* @var altitude
**/

struct Cloud {
    std::string type;
    int altitude;
};

/**
	* @struct Wind represent abstract base of winds
**/

struct Wind {
	/** \var direction **/
    int direction;
	
	/** \var speed **/
    int speed;
};

class Metar {
public:
    Metar(std::shared_ptr <Settings> settings);
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
	std::string id_code;
	void generate_clouds();
	std::time_t time;
	
	void generate_visibility();
	void generate_wind();
	void generate_humidity();
	
	std::shared_ptr <Settings> settings;
};

#endif // METAR_HPP
