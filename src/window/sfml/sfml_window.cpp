#include "sfml_window.hpp"

SFML_window::SFML_window() { }

SFML_window::~SFML_window() { }

void SFML_window::init() {
	#ifdef DEBUG
    std::clog << "SFML_window::init()" << std::endl;
	#endif
	
	this->load_settings();
	
	this->time_change = sf::milliseconds(this->settings->time_change);
}

void SFML_window::run() {
	#ifdef DEBUG
	std::clog << "SFML_window::run()" << std::endl;
	#endif
	
	sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
	
	sf::RenderWindow window(
		sf::VideoMode(
			this->settings->screen_width, 
			this->settings->screen_height,
			desktop.bitsPerPixel), 
		this->settings->program_name, 
		sf::Style::Close
	);

	sf::Image image;
	
	if (image.loadFromFile(this->settings->logo_image)) {
		window.setIcon(image.getSize().x, image.getSize().y, image.getPixelsPtr());
	} 

	sfml_drawsurface drawer(window);
	
	Gamecontroller gamecontroller(this->settings, drawer);
	
	gamecontroller.load();
	sf::Time time_now;
	sf::Time game_time;
	
	sf::Event event;
	bool on_run = true;
	bool drawed = false;
	
	game_time = sf::milliseconds(1);
	
	while (on_run) {
		while (window.pollEvent(event) && on_run) {
			on_run = this->handle_event(event, gamecontroller, window);
		}
		
		time_now = this->clock.getElapsedTime();
		
		while (game_time + time_change <= time_now) {
			gamecontroller.update(time_change.asMilliseconds());
			
			game_time += time_change;
			
			drawed = false;
		}
		
		if (drawed) {
			sf::sleep(sf::milliseconds(1));
		} else {
			gamecontroller.draw(mouse);
			drawed = true;
		}
	}
}

bool SFML_window::handle_event(sf::Event& event, Controller& ctrl, sf::RenderWindow& window) {
	#ifdef DEBUG
	std::clog << "SFML_window::handle_event()" << std::endl;
	#endif
	
    sf::Vector2i mouse_place = sf::Mouse::getPosition(window);
	mouse.set_place(mouse_place.x, mouse_place.y);
	
	sf::Time time_pressed_first;
	sf::Time time_pressed_again;
		
    switch (event.type) {
        case sf::Event::Closed:
            return false;
			
			break;
		 case sf::Event::TextEntered:
			if (event.text.unicode == '\b' && this->input_string.length()) {
                this->input_string = this->input_string.erase(input_string.length()-1, 1);
            } else if (event.text.unicode != 13 && event.text.unicode < 128) {
                this->input_string += sf::String(event.text.unicode);
            }
			
			ctrl.update_command(this->input_string);
            return true;
			
			break;
        case sf::Event::MouseWheelMoved:
            ctrl.handle_mouse_wheel(event.mouseWheel.delta);
			return true;
			
			break;
        case sf::Event::KeyPressed:
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return)) {
                ctrl.handle_text_input();
                this->input_string = "";
            } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
                return false;
            } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Tab)) {
				ctrl.handle_function_keys(sf::Keyboard::Tab);
			} else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
				ctrl.handle_function_keys(sf::Keyboard::Down);
			} else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
				ctrl.handle_function_keys(sf::Keyboard::Up);
			} 
            
			return true;
			
			break;
        case sf::Event::MouseButtonPressed:
            if (event.mouseButton.button == sf::Mouse::Left) {
                mouse_start.set_place(mouse_place.x, mouse_place.y);
                ctrl.handle_mouse_click(mouse_start);
            }
			
			return true;
			
			break;
        case sf::Event::MouseMoved:
            return true;
			
			break;
        case sf::Event::MouseButtonReleased:
            mouse_end.set_place(mouse_place.x, mouse_place.y);
			
			ctrl.handle_mouse_release(mouse_start, mouse_end);
			
			return true;
			
			break;
		case sf::Event::Resized:	
			this->settings->screen_width = event.size.width;
			this->settings->screen_height = event.size.height;

			return true;
			
			break;
        default:
            return true;
			
			break;
    }
	
    return true;
}
