#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "../tools/settings.hpp"

class Player {
public:
    Player(Settings& s);
    ~Player();
    void update(double elapsed);
protected:
private:
    Settings& settings;
};


#endif // PLAYER_HPP
