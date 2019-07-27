#ifndef _QUERYROW_HPP
#define _QUERYROW_HPP

#include <vector>
#include <iostream>
#include <sqlite3pp.h>

#include "tables/airline.hpp"

/**
    * Queryrow helper class for sqlite3pp. It provides easier access to results.
	* @todo make class for database table. 
**/

class Queryrow {
public:
    /**
        Queryrow consructor
        @param std::string row normal string with spaces between values
        @return none
    **/
    explicit Queryrow(std::string row);
	
    ~Queryrow();
	
    /**
        * fetch
        * Your get normal vector with all result. It's not ment to use direcly.
        * You should use Queryresult to access to the data.
        * @param none
        * @return std::vector <std::string>
    **/
    std::vector <std::string> fetch();
protected:
private:
    std::string values;
    std::vector <std::string> tokens;
};

#endif // _QUERYROW_HPP
