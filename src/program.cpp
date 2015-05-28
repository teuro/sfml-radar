#include "program.hpp"

Program::Program(Gamecontroller& gc,
                 Atiscontroller& ac,
                 Playercontroller& pc,
                 sf::RenderWindow& w,
                 Settings& s) :
                     gamecontroller(gc),
                     atiscontroller(ac),
                     playercontroller(pc),
                     window(w),
                     settings(s){
    this->load();
    this->game_state = PLAYER;
    this->counter = 30;
}

Program::~Program() { }

void Program::init() {
    std::clog << "Program::init()" << std::endl;
    this->gamecontroller.load();

    Queryresult qrslt = Database::get_result("SELECT setting_name, setting_value FROM settings");
    std::map <std::string, std::string> tmp;

    for (unsigned int i = 0; i < qrslt.size(); ++i) {
        tmp[qrslt(i, "setting_name")] = qrslt(i, "setting_value");
    }

    this->settings.set_values(tmp);

    this->window.setTitle(settings.program_name);
}

void Program::close() {
    std::clog << "Program::close()" << std::endl;
}

bool Program::run() {
    sf::Event event;
    sf::Time elapsed = this->clock.restart();

    switch (this->game_state) {
    case GAME:
        this->ctrl = &this->gamecontroller;
        break;
    case ATIS:
        this->ctrl = &this->atiscontroller;
        break;
    case PLAYER:
        this->ctrl = &this->playercontroller;
        break;
    }

    while (this->window.pollEvent(event)) {
        return this->handle_event(event);
    }

    if (this->counter > 20 && !this->draw) {
        this->counter = 0;
        this->draw = true;
    } else {
        sf::sleep(sf::milliseconds(50));
        ++this->counter;
        this->draw = false;
    }

    this->ctrl->update(elapsed.asMilliseconds(), this->draw);

    if (this->ctrl->is_ok()) {
        if (this->game_state == PLAYER) {
            this->game_state = ATIS;
        } else if (this->game_state == ATIS) {
            this->game_state = GAME;
        }
    }

    return true;
}

bool Program::handle_event(sf::Event& event) {
    sf::Vector2i mouse_place = sf::Mouse::getPosition(this->window);

    switch (event.type) {
        case sf::Event::Closed:
            this->window.close();
            return false;
        case sf::Event::KeyPressed:
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return)) {
                this->ctrl->handle_text_input();
                this->input_string = "";
            } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
                this->window.close();
                return false;
            } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
				this->ctrl->handle_function_keys(Tools::UP);
			} else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
				this->ctrl->handle_function_keys(Tools::DOWN);
			} else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
				this->ctrl->handle_function_keys(Tools::LEFT);
			} else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
				this->ctrl->handle_function_keys(Tools::RIGHT);
			}
			
			if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Return)) {
				input_string = this->ctrl->get_input();
			}
            return true;
        case sf::Event::TextEntered:
            if (event.text.unicode == 8 && this->input_string.length()) {
                input_string = input_string.erase(input_string.length()-1, 1);

            } else if (
				((event.text.unicode >= 65 && event.text.unicode <= 122) ||
				(event.text.unicode >= 48 && event.text.unicode <= 57) ||
				(event.text.unicode == 32)) && 
				event.text.unicode != 13 && event.text.unicode != 8) {
                input_string += sf::String(event.text.unicode);
            }

            this->ctrl->update_command(this->input_string);
            return true;
        case sf::Event::MouseWheelMoved:
            this->ctrl->handle_mouse_wheel(event.mouseWheel.delta);
            break;
        case sf::Event::MouseButtonPressed:
            if (event.mouseButton.button == sf::Mouse::Left) {
                mouse_start.set_place(mouse_place.x, mouse_place.y);
                this->ctrl->handle_mouse_click(mouse_start);
            }
        case sf::Event::MouseMoved:
            return true;
        case sf::Event::MouseButtonReleased:
            mouse_end.set_place(mouse_place.x, mouse_place.y);
            this->ctrl->handle_mouse_release(mouse_start, mouse_end);
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

void Program::load() {
    std::clog << "Program::load()" << std::endl;
}
