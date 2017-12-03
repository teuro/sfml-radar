#ifndef _TEXT_TOOLS_HPP
#define _TEXT_TOOLS_HPP

#include "tools.hpp"

namespace Tools {
	/**
		* is_match
		* Checks if two strings are similar
		* @param std::string a
		* @param std::string b
		* @return bool
	**/
	
	bool is_match(std::string a, std::string b);
	
	/**
		* split
		* explodes string to pieces
		* @param std::string delimiter
		* @param std::string input
		* @return std::vector <std::string>
	**/
	
	std::vector <std::string> split(std::string delimiter, std::string input);
	
	/**
		* ltrim
		* trims spaces from start of string
		* @param std::string s
		* @return std::string
	**/

	std::string ltrim(std::string s);
	
	/**
		* ltrim
		* trims spaces from end of string
		* @param std::string s
		* @return std::string
	**/
	
	std::string rtrim(std::string s);
	
	/**
		* ltrim
		* trims spaces from start and end of string
		* @param std::string s
		* @return std::string
	**/
	
	std::string trim(std::string s);
	
	/**
		* replace
		* Loops throug map and make replace each loop
		* @param std::string original
		* @param std::map <std::string, std::string> repl
		* @return std::string
	**/
	
	std::string replace(std::string original, std::map <std::string, std::string> repl);
	
	/**
		* replace
		* Finds specific string and replace it with other string
		* @param std::string original
		* @param std::string 
		* @param std::string 
		* @return std::string
	**/
	
	std::string replace(std::string original, std::string find, std::string replace);
	
	/**
		* tostr
		* converts numbers to string
		* @param T might bee any number
		* @return std::string
	**/
	
	template <class T>
	std::string tostr(T a) {
        std::ostringstream os;

        os << a;

        return os.str();
	}
	
	/**
		* tostr
		* converts numbers to constant length string
		* @param T might bee any number
		* @param unsigned int length 
		* @return std::string
	**/
	
	template <class T>
	std::string tostr(T a, unsigned int length) {
        std::ostringstream os;

        os << a;
		
		std::string t = os.str();
		
		if (t.length() < length) {
			unsigned int required = length - t.length();
			std::string fill_zeros;
			
			for ( unsigned int i = 0; i < required; ++i) {
				fill_zeros += "0";
			}
			
			t = fill_zeros + t;
		}

        return t;
	}
}

#endif