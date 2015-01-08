#include <ctime>
#include <iostream>
#include <cstdlib>
#include <stdexcept>

#include "program.hpp"

int main(){
    std::srand(std::time(NULL));
    try {
        sf::RenderWindow window(sf::VideoMode(800, 600), "ATC-radar");
        sfml_drawsurface drawer(window);

        Coordinate cp(62.0000, 25.0000);
        Settings settings(cp);

        Game    game    (cp, settings);
        Atis    atis    (settings);
        Player  player  (settings);

        Gameview    gameview(drawer, settings);
        Atisview    atisview(atis,      drawer, settings);
        Playerview  playerview(player,  drawer, settings);

        Gamecontroller      gamecontroller(     gameview,       settings, game);
        Atiscontroller      atiscontroller(     atisview,       settings, atis );
        Playercontroller    playercontroller(   playerview,     settings, player);

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
    } catch ( ... ) {
        std::cerr << "Unknown error" << std::endl;
        return 1;
    }

    return 0;
}
