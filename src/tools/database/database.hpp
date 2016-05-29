#ifndef DATABASE_HPP
#define DATABASE_HPP

#include <map>
#include <string>
#include <stdexcept>

#include "queryresult.hpp"
#include "../settings.hpp"

/**
    * namespace Database
    * Tiny wrapper interface to gather data
	* Database does NOT storage any data, so You just receive raw data.
**/

namespace Database {
	/**
        * get_result
        * Make a query and returns data
		* Makes absolutely anything to results, so You receive raw data from database
        * @param std::string query
        * @return Queryresult
    **/
	
    Queryresult get_result(std::string query);

    /**
        * bind_param
        * Adds specified variables to query string
		* You may specify Your own search term and bind term if it's nessecery, but defaults works just fine as well.
		* Technically it would be better to define search and bind terms somewhere else, but for now this is more than enough
        * @param std::string query
        * @param std::stack variables
        * @return std::string
    **/

    std::string bind_param(std::string query_string, std::map <std::string, std::string> variables);
}

#endif // DATABASE_HPP