#ifndef _SFML_WINDOW
#define _SFML_WINDOW

#include "../window.hpp"

#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <fstream>
#include <iostream>
#include <list>
#include <tools/tools.hpp>

class SFML_window : public Window {
public:
	SFML_window();
	~SFML_window();
	virtual void init() override;
	virtual void run() override;
private:
	virtual bool handle_event(sf::Event& event, Controller& ctrl, sf::RenderWindow& window);
    sf::Clock clock;
	sf::Time time_change;
};

#endif
