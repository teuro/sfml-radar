#include "airline.hpp"

Airline_table::Airline_table(std::string ic, std::string n) : Database_table("airlines"), icao(ic), name(n) { }

Airline_table::~Airline_table() { }
