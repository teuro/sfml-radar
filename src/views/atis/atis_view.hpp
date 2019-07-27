#ifndef ATIS_VIEW_HPP
#define ATIS_VIEW_HPP

#include <vector>
#include <iostream>

#include "../view.hpp"
#include "../../tools/tools.hpp"
#include "../../tools/point_tools.hpp"
#include "../../tools/settings.hpp"
#include "../../models/weather/atis.hpp"
#include "../../models/game_objects/runway.hpp"

/**
	* Atis_view 
	* Is responsible for display atis. 
	* 
**/

class Atisview : public View {
public:
	/**
		* Atisview 
		* Constructor to settle everything up
		* @param drawer to draw 
		* @param settings
		* @param atis 
		* @return void
	**/
	
	Atisview(Drawsurface& drawer, std::shared_ptr <Settings> settings, std::shared_ptr <Atis> atis);
	~Atisview();
	
	/**
		* draw
		* Draw atis 
		* @param mouse 
		* @return void
	**/
	
	virtual void draw(Point& mouse) override;
	
	/**
		* handle_click
		* NOT IMPLEMENTED YET
		* @param mouse
		* @return std::string
	**/
	
	virtual std::string handle_click(Point& mouse) override;
    void load() override;
	std::string get_value();
	void draw_errors();
	virtual void update() override;
private:
	std::shared_ptr <Atis> atis;
};

#endif // ATIS_VIEW
