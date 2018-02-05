#ifndef _GAME_VIEW_HPP
#define _GAME_VIEW_HPP

#include <vector>
#include <list>
#include <map>
#include <iostream>

#include "../view.hpp"
#include "../../tools/tools.hpp"
#include "../../tools/point_tools.hpp"
#include "../../tools/settings.hpp"
#include "../../models/coordinate/coordinate.hpp"
#include "../../models/coordinate/navpoint.hpp"
#include "../../models/game_objects/aircraft.hpp"
#include "../../models/game_objects/airfield.hpp"
#include "../../models/game.hpp"

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
	
	Gameview(Drawsurface& drawsurface, std::shared_ptr <Settings> settings, std::shared_ptr <Game> game);
	
	/**
		* Gameview desstructor
		* @return void
	**/
	~Gameview();
	
	/**
		* Gameview draw
		* Places logo to screen and calls upper class View::draw() for basic elements
		* @param Point& mouse
		* @return void
	**/
	
	virtual void draw(Point& mouse);
	
	virtual std::string handle_click(Point& mouse);
	
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
	
	void draw_airfield(std::shared_ptr <Airfield> airfield);
	
	/**
		* Gameview draw_planes
		* Draws all planes to the screen
		* @todo don't draw if plane is not in screen
		* @param std::list <Aircraft*> planes
		* @return void
	**/
	
	void draw_planes(std::list <Aircraft*> planes, Aircraft* selected, Point& mouse);
	
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
	 virtual void update();
	 
	 double distancePX(double nautical);
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
	bool loaded;
	
	std::shared_ptr <Game> game;
};

#endif // _GAME_VIEW_HPP
