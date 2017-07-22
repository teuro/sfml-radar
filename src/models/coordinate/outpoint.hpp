#ifndef OUTPOINT_HPP
#define OUTPOINT_HPP

#include "navpoint.hpp"

class Outpoint : public Navpoint {
public:
    Outpoint(std::string name, Coordinate p);
    Outpoint(std::string name, double latitude, double longitude);
    ~Outpoint();
protected:
private:
};

#endif // OUTPOINT_HPP
