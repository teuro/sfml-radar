#ifndef _SFML_WINDOW
#define _SFML_WINDOW

#include "../window.hpp"

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

class SFML_window : public Window {
public:
	SFML_window();
	~SFML_window();
	virtual void init();
	virtual void close();
	virtual void run();
	virtual void load_settings();
	virtual void handle_events();
private:
	virtual bool handle_event(sf::Event& event, Controller& ctrl, sf::RenderWindow& window);
    sf::Clock clock;
	sf::Time time_change;
};

#endif
