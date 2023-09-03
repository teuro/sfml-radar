#include <iostream>
#include <stdexcept>

#include "setup/database_setup.hpp"
#include "window/sfml/sfml_window.hpp"

/**
	* ATC radar game
	* This is main function for the game. Basically this just initialize program and catch exceptions. 
	* 
**/

int main(int argc, char** argv) {
	if (argc > 1) {
		if (strcmp(argv[1], "-ct") == 0) {
			Setup::create_tables();
		}
	}
	
    try {
        SFML_window window;
		
		window.init();
		
        window.run();
    } catch (std::runtime_error& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    } catch (std::logic_error& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    } catch (std::bad_alloc& e) {
        std::cerr << e.what() << std::endl;
		return 1;
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
		return 1;
    } catch ( ... ) {
        std::cerr << "Unknown error" << std::endl;
        return 1;
    }

    return 0;
}
