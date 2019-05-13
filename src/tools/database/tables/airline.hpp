#ifndef _AIRLINE_TABLE_CLASS_HPP
#define _AIRLINE_TABLE_CLASS_HPP

#include <string>

#include "../database_table.hpp"

class Airline_table : public Database_table {
	std::string icao;
	std::string name;
public:
	Airline_table(std::string icao, std::string name);
	~Airline_table();
};

#endif
