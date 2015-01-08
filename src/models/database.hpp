#ifndef DATABASE_HPP
#define DATABASE_HPP

#include <stack>
#include <string>

#include "../tools/database/queryresult.hpp"

/**
    * namespace Database
    * Tiny wrapper interface to gather data
**/

namespace Database {
    /**
        * get_result
        * Make a query and returns data
        * @param std::string query
        * @return Queryresult
    **/
    Queryresult get_result(std::string query);

    /**
        * bind_param
        * Adds specified variables to query string
        * @param std::string query
        * @param std::stack variables
        * @return std::string
    **/

    std::string bind_param(std::string query_string, std::stack <std::string> variables);
}

#endif // DATABASE_HPP
