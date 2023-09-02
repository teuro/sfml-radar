#include "sfml_window.hpp"

SFML_window::SFML_window() { }

SFML_window::~SFML_window() { }

void SFML_window::init() {
	#ifdef DEBUG
    std::clog << "SFML_window::init()" << std::endl;
	#endif
	
	this->time_change = sf::milliseconds(100);
	
	this->load_settings();
}

void SFML_window::load_settings() {
	#ifdef DEBUG
	std::clog << "SFML_window::load_settings()" << std::endl;
	#endif
	
	std::shared_ptr <Settings> s (new Settings);
	this->settings = s;
	
	Tools::init_random();
	std::map <std::string, std::string> tmp;
	std::string line;
	//std::string type;
	std::string name;
	std::string value;
	
	std::ifstream setting_file("settings/settings.ini", std::ios::in);
	
	while (std::getline(setting_file, line)) {
		std::vector <std::string> cells = Tools::split(" ", line);
		
		if (cells.size() == 3) {
			//type = cells[0];
			name = cells[1];
			value = cells[2];
			
			tmp[name] = value;
		}
	}
	
	setting_file.close();

	this->settings->set_values(tmp);
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
	
	sf::Event event;
	bool on_run = true;
	std::list <int> waits;
	unsigned int round = 0;
	unsigned int rounds = 50;
	
	while (on_run) {
		while (window.pollEvent(event) && on_run) {
			on_run = this->handle_event(event, gamecontroller, window);
		}
		
		time_now = this->clock.restart();
		int wait = (time_change - time_now).asMilliseconds();
		
		if (wait) {
			waits.push_back(wait);
		}
		
		//std::clog << "Sleeping " << Tools::average(waits) << " ms" << std::endl;
		
		if (waits.size()) {
			if (waits.size() > rounds) {
				waits.pop_front();
			}
			
			if (round == rounds) {
				if (Tools::average(waits) > 20) {
					this->time_change -= sf::milliseconds(1);
				} else if (Tools::average(waits) < 15) {
					this->time_change += sf::milliseconds(10);
				}
				
				round = 0;
			}
		}
		
		gamecontroller.update(time_change.asMilliseconds(), mouse);
		
		sf::sleep(sf::milliseconds(wait));
		++round;
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