#include <iostream>
#include <stdexcept>

#include "program.hpp"

int main(){
    try {
        Program program;
		
		program.init();
		
        program.run();

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
