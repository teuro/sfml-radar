#ifndef QUERYRESULT_HPP
#define QUERYRESULT_HPP

#include "queryrow.hpp"

#include <vector>

/**
    Queryresult provides quite easy access to gathered data from SQL
**/

class Queryresult {
public:
    /**
        * Queryresult constructor
        * @param sqlite3pp::query query normal query
        * @return none
    **/
    Queryresult(sqlite3pp::query& query);
    Queryresult();

    ~Queryresult();

    /**
        * operator() place were macig begins
        * @param unsigned int row needed row
        * @param std::string column requested column name
        * @return std::string basically one value (std::string for a while)
        * @todo tempalate for others formats might be good idea.
    **/
	
    std::string operator()(unsigned int row, std::string column);
	
	 /**
        * operator() place were macig begins
        * @param unsigned int row requested row
        * @param unsigned int column requested column
        * @return std::string basically one value (std::string for a while)
        * @todo tempalate for others formats might be good idea.
    **/
	
    std::string operator()(unsigned int row, unsigned int column);

	 /**
        * operator[] place were macig begins
        * @param unsigned int row needed row
        * @return Queryrow needs handling before use
    **/
	
    Queryrow operator[](unsigned int row);
	
    std::size_t size();
protected:
private:
    std::vector <Queryrow> queryrows;
    std::vector <std::string> headers;
};
#endif // QUERYRESULT_HPP
