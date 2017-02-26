#ifndef _GAME_VIEW_HPP
#define _GAME_VIEW_HPP

#include <vector>
#include <list>
#include <map>
#include <iostream>

#include "../view.hpp"
#include "../../tools/tools.hpp"
#include "../../tools/settings.hpp"
#include "../../models/coordinate.hpp"
#include "../../models/navpoint.hpp"
#include "../../models/aircraft.hpp"
#include "../../models/airfield.hpp"

class Drawable_plane : public Drawable_element {
public:
	Drawable_plane(std::string callsign, std::string t_name, std::string t_class, std::string t_id);
	void set_callsign(std::string);
	std::string get_callsign();
private:
	std::string callsign;
};

class Drawable_navpoint : public Drawable_element {
public:
	Drawable_navpoint(std::string name, std::string t_name, std::string t_class, std::string t_id);
	std::string get_name();
private:
	std::string name;
};

/**
	* Gameview is responsible to render game to screen.
	* view 
**/

class Gameview : public View {
public:
	/**
		* Gameview constructor
		* @param Drawsurface d
		* @param Settings s
		* @return void
	**/
	
	Gameview(Drawsurface& d, Settings& s);
	
	/**
		* Gameview desstructor
		* @return void
	**/
	~Gameview();
	
	/**
		* Gameview draw
		* Places logo to screen and calls upper class View::draw() for basic elements
		* @param void
		* @return void
	**/
	
	void draw();
	
	/**
		* Gameview load
		* Sets centerpoint from settings and calls upper class View::load()
		* @param void
		* @return void
	**/
	
	void load();
	
	/**
		* Gameview draw_airfield
		* Draws current airfield runways basically
		* @param Airfield* airfield
		* @return void
	**/
	
	void draw_airfield(Airfield* airfield);
	
	/**
		* Gameview draw_planes
		* Draws all planes to the screen
		* @todo don't draw if plane is not in screen
		* @param std::list <Aircraft*> planes
		* @return void
	**/
	
	void draw_planes(std::list <Aircraft*> planes, Aircraft* selected, Point& mouse);
	
	void set_zoom(int zoom);
	
	/**
		* calculate
		* Calculates new screen point
		* @param Point center_point_screen
		* @param Coordinate center_point_map
		* @param Coordinate target
		* @param int zoom
	**/
	
	Point calculate(Coordinate& target);
	
	/**
		draw many elements
	**/
	
	 void update_command(std::string command);
	 
	 double distancePX(double nautical);
	 double distanceNM(double pixels);
	 void calculate_coordinate_limits(double distance);
private:
	
	/**
		* Gameview draw_plane
		* Draw one plane 
		* @param Aircraft*& plane 
		* @param std::string color
		* @return void
	**/
	
    void draw_plane(Aircraft*& plane, Aircraft* selected, Point& mouse);
	std::string command;
};

#endif // _GAME_VIEW_HPP
