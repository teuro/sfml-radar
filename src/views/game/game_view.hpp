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
		* Gameview draw_navpoints
		* Draws all navigation points which are linked to the airfield
		* @param std::vector <Navpoint>& navpoints
		* @return void
	**/
	
	void draw_navpoints(std::vector <Navpoint>& navpoints);
	
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
	
	void draw_planes(std::list <Aircraft*> planes);
	
	/**
		* Gameview set_centerpoint
		* Sets centerpoint to the frame
		* @param Coordinate& centerpoint
		* @return void
	**/
	
	void set_centerpoint_map(Coordinate& centerpoint_map);
	
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
private:
	/**
		* Gameview centerpoint
		* Centerpoint on map
	**/
	
	Coordinate centerpoint_map;
	
	/**
		* Gameview draw_plane
		* Draw one plane 
		* @param Aircraft*& plane 
		* @param std::string color
		* @return void
	**/
	
    void draw_plane(Aircraft*& plane);
	
	Point text_callsign;
	Point text_speed;
	Point text_altitude;
	Point text_heading;
};

#endif // _GAME_VIEW_HPP
