#ifndef _SETTINGS_HPP
#define _SETTINGS_HPP

#include <map>
#include <string>
#include "tools.hpp"

class Settings {
public:
	Settings();
	~Settings();
	void set_values(std::map <std::string, std::string> values);

    int screen_width;
    int screen_height;
    int clearance_height_lower;
    int clearnace_height_upper;
    int shortcut;
    int separation_horizontal;
    int separation_vertical;
    int heading_change;
    int height_change;
    int speed_change;
    int distance_width;
    int clearance_speed_lower;
    int clearance_speed_upper;
    int new_plane_lower;
    int new_plane_upper;
    int required_handled;
    int max_planes;
    int approach_point_distance;
    int speed_reduction_point;
    int max_separation_errors;
    int max_approach_speed;
    int approach_angle;
    int max_approach_altitude;
    int approach_speed;
    int final_approach_speed;
    int glide_slope;
    int initial_speed;
    int metar_update;
    int speed_restriction_altitude;
    int fuel_consumption;
    int fuel_minima;
    int cloud_altitude_lower;
    int cloud_altitude_upper;
    int wind_lower;
    int wind_upper;
    int air_pressure_lower;
    int air_pressure_upper;
    int visibility_lower;
    int visibility_upper;
    int temperatur_lower;
    int temperatur_upper;
    int humidity_lower;
    int humidity_upper;
    int cloud_count_lower;
    int cloud_count_upper;
    int departure_separation;
    int zoom;
	int airfield_altitude;
	int sleep;
    std::string program_name;
    std::string database_name;
    std::string search_term;
    std::string bind_term;
	std::string layout_file_name;
	Coordinate centerpoint;
};


#endif
