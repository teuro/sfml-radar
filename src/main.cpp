#include <iostream>
#include <stdexcept>

#include "program.hpp"

int main(){
	Tools::init_random();

    try {
		sf::RenderWindow window(sf::VideoMode(800, 600), "ATC-radar");
        sfml_drawsurface drawer(window);

        Settings settings;
	
        Atis    atis    (settings);
		
        Gamecontroller      gamecontroller(drawer, settings, atis);
        Atiscontroller      atiscontroller(drawer, settings, atis);
        Playercontroller    playercontroller(drawer, settings);

        Program program(gamecontroller, atiscontroller, playercontroller, window, settings);
		program.init();
		
        while (program.run()) { }

        program.close();

    } catch (std::runtime_error& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    } catch (std::logic_error& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    } catch (std::bad_alloc& e) {
        std::cerr << e.what() << std::endl;
		return 1;
    } catch ( ... ) {
        std::cerr << "Unknown error" << std::endl;
        return 1;
    }

    return 0;
}
