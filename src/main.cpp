#include <iostream>
#include <stdexcept>

#include "window/sfml/sfml_window.hpp"

/**
	* ATC radar game
	* This is main function for the game. Basically this just initialize program and catch exceptions. 
	* 
**/

int main(){
    try {
        SFML_window window;
		
		window.init();
		window.load_settings();
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
