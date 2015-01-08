#ifndef ATIS_HPP
#define ATIS_HPP

#include "../tools/settings.hpp"

/**
    Atis presenting user inputted data which defines active runways and transition altitude and level.
**/

class Atis {
public:
    /**
        * Constructor
        * @param Settings
        * @return none
    **/
    Atis(Settings& s);
    ~Atis();
    /**
        * Update updates this based on time from controller.
        * @param double elapsed
        * @return void
    **/
    void update(double elapsed);
protected:
private:
    Settings& settings;
};


#endif // ATIS_HPP
