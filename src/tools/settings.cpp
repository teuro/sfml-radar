#include "settings.hpp"

Settings::Settings() { }

Settings::~Settings() { }

void Settings::set_values(std::map<std::string, std::string> values ) {
    this->screen_width                  = Tools::toint(values["screen_width"]);
    this->screen_height                 = Tools::toint(values["screen_height"]);
    this->clearance_height_lower        = Tools::toint(values["clearance_height"]);
    this->clearnace_height_upper        = Tools::toint(values["clearnace_height"]);
    this->shortcut                      = Tools::toint(values["shortcut"]);
    this->separation_horizontal         = Tools::toint(values["separation_horizontal"]);
    this->separation_vertical           = Tools::toint(values["separation_vertical"]);
    this->heading_change                = Tools::toint(values["heading_change"]);
    this->height_change                 = Tools::toint(values["height_change"]);
    this->speed_change                  = Tools::toint(values["speed_change"]);
    this->distance_width                = Tools::toint(values["distance_width"]);
    this->clearance_speed_lower         = Tools::toint(values["clearance_speed"]);
    this->clearance_speed_upper         = Tools::toint(values["clearance_speed"]);
    this->new_plane_lower               = Tools::toint(values["new_plane_lower"]);
    this->new_plane_upper               = Tools::toint(values["new_plane_upper"]);
    this->required_handled              = Tools::toint(values["required_handled"]);
    this->max_planes                    = Tools::toint(values["max_planes"]);
    this->approach_point_distance       = Tools::toint(values["approach_point"]);
    this->speed_reduction_point         = Tools::toint(values["speed_reduction"]);
    this->max_separation_errors         = Tools::toint(values["max_separation"]);
    this->max_approach_speed            = Tools::toint(values["max_approach"]);
    this->approach_angle                = Tools::toint(values["approach_angle"]);
    this->max_approach_height           = Tools::toint(values["max_approach"]);
    this->approach_speed                = Tools::toint(values["approach_speed"]);
    this->final_approach_speed          = Tools::toint(values["final_approach"]);
    this->glide_slope                   = Tools::toint(values["glide_slope"]);
    this->initial_speed                 = Tools::toint(values["initial_speed"]);
    this->metar_update                  = Tools::toint(values["metar_update"]);
    this->speed_restriction_altitude    = Tools::toint(values["speed_restriction"]);
    this->fuel_consumption              = Tools::toint(values["fuel_consumption"]);
    this->fuel_minima                   = Tools::toint(values["fuel_minima"]);
    this->cloud_altitude_lower          = Tools::toint(values["cloud_altitude"]);
    this->cloud_altitude_upper          = Tools::toint(values["cloud_altitude"]);
    this->wind_lower                    = Tools::toint(values["wind_lower"]);
    this->wind_upper                    = Tools::toint(values["wind_upper"]);
    this->air_pressure_lower            = Tools::toint(values["air_pressure_lower"]);
    this->air_pressure_upper            = Tools::toint(values["air_pressure_upper"]);
    this->visibility_lower              = Tools::toint(values["visibility_lower"]);
    this->visibility_upper              = Tools::toint(values["visibility_upper"]);
    this->temperatur_lower              = Tools::toint(values["temperatur_lower"]);
    this->temperatur_upper              = Tools::toint(values["temperatur_upper"]);
    this->humidity_lower                = Tools::toint(values["humidity_lower"]);
    this->humidity_upper                = Tools::toint(values["humidity_upper"]);
    this->cloud_count_lower             = Tools::toint(values["cloud_count_lower"]);
    this->cloud_count_upper             = Tools::toint(values["cloud_count_upper"]);
    this->departure_separation          = Tools::toint(values["departure_separation"]);
    this->zoom                          = 10;
    this->program_name                  = values["program_name"];
}
