#ifndef DATABASE_HPP
#define DATABASE_HPP

#include <map>
#include <list>
#include <string>
#include <stdexcept>

#include "queryresult.hpp"
#include <tools/text_tools.hpp>

/**
    * Class Database
    * This is responsible for every database actions
**/

class Database {
public:
	Database();
	
	int insert(std::string table_name, std::list <std::string> columns, std::list <std::string> values);
	void update(std::string table_name, std::map <std::string, std::string> values, std::string condition);
	
	/**
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
        * @param query_string
        * @param variables
        * @return std::string
    **/

    std::string bind_param(std::string query_string, std::map <std::string, std::string> variables);
	
	 /**
        * bind_param
        * Adds specified variable to query string
		* You may specify Your own search term and bind term if it's nessecery, but defaults works just fine as well.
		* Technically it would be better to define search and bind terms somewhere else, but for now this is more than enough
        * @param query_string
        * @param variables
        * @return std::string
    **/
	
    std::string bind_param(std::string query_string, std::list <std::string> variables);
};

#endif // DATABASE_HPP
