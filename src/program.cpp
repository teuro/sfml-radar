#include "program.hpp"

Program::Program(){
    this->load();
	this->time_change = sf::milliseconds(50);
}

Program::~Program() { }

void Program::init() {
    std::clog << "Program::init()" << std::endl;
	
	Tools::init_random();
	
	Queryresult qrslt = Database::get_result("SELECT setting_name, setting_value FROM settings");
	std::map <std::string, std::string> tmp;

	for (unsigned int i = 0; i < qrslt.size(); ++i) {
		tmp[qrslt(i, "setting_name")] = qrslt(i, "setting_value");
	}

	settings.set_values(tmp);
}

void Program::load() {
    std::clog << "Program::load()" << std::endl;
}

void Program::close() {
    std::clog << "Program::close()" << std::endl;
}

bool Program::handle_events(Controller& ctrl, sf::RenderWindow& window) {
	sf::Event event;

	while (window.pollEvent(event)) {
		return this->handle_event(event, ctrl, window);
	}
	
	return true;
}

void Program::run() {
	sf::RenderWindow window(sf::VideoMode(this->settings.screen_width, this->settings.screen_height), this->settings.program_name);
	sfml_drawsurface drawer(window);
	
	Gamecontroller gamecontroller(this->settings, drawer);
	gamecontroller.load();
	sf::Time time_now;
	
	while (this->handle_events(gamecontroller, window)) {
		time_now = this->clock.restart();
		gamecontroller.update(time_now.asMilliseconds(), mouse);
		sf::sleep(time_change - time_now);	
	}
	
	window.close();
}

bool Program::handle_event(sf::Event& event, Controller& ctrl, sf::RenderWindow& window) {
    sf::Vector2i mouse_place = sf::Mouse::getPosition(window);
	mouse.set_place(mouse_place.x, mouse_place.y);
	int button_type = -1;

    switch (event.type) {
        case sf::Event::Closed:
            return false;
        case sf::Event::KeyPressed:
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return)) {
                ctrl.handle_text_input();
                this->input_string = "";
				ctrl.update_command(this->input_string);
            } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
                return false;
            } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
				button_type = Tools::UP;
			} else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
				button_type = Tools::DOWN;
			} else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
				button_type = Tools::LEFT;
			} else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
				button_type = Tools::RIGHT;
			}
			this->input_string = ctrl.handle_function_keys(button_type);
            return true;
        case sf::Event::TextEntered:
            if (event.text.unicode == 8 && this->input_string.length()) {
                this->input_string = this->input_string.erase(input_string.length()-1, 1);
            } else if (
				((event.text.unicode >= 65 && event.text.unicode <= 122) ||
				(event.text.unicode >= 48 && event.text.unicode <= 57) ||
				(event.text.unicode == 32 || event.text.unicode == 58)) && 
				event.text.unicode != 13 && event.text.unicode != 8) {
                this->input_string += sf::String(event.text.unicode);
            }
			
			ctrl.update_command(this->input_string);
            return true;
        case sf::Event::MouseWheelMoved:
            ctrl.handle_mouse_wheel(event.mouseWheel.delta);
            break;
        case sf::Event::MouseButtonPressed:
            if (event.mouseButton.button == sf::Mouse::Left) {
                mouse_start.set_place(mouse_place.x, mouse_place.y);
                ctrl.handle_mouse_click(mouse_start);
            }
        case sf::Event::MouseMoved:
            return true;
        case sf::Event::MouseButtonReleased:
            mouse_end.set_place(mouse_place.x, mouse_place.y);
            ctrl.handle_mouse_release(mouse_start, mouse_end);
            return true;
            break;
        case sf::Event::Resized:
            this->settings.screen_height = event.size.height;
            this->settings.screen_width = event.size.width;
        default:
            return true;
    }

    return true;
}
