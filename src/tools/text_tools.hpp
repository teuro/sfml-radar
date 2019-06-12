#ifndef _TEXT_TOOLS_HPP
#define _TEXT_TOOLS_HPP

#include "tools.hpp"

namespace Tools {
	/**
		* is_match
		* Checks if two strings are similar
		* @param std::string input_number
		* @param std::string b
		* @return bool
	**/
	
	bool is_match(std::string input_number, std::string b);
	
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
	std::string tostr(T input_number) {
        std::ostringstream outstringstream;

        outstringstream << input_number;

        return outstringstream.str();
	}
	
	/**
		* tostr
		* converts numbers to constant length string
		* @param T might bee any number
		* @param unsigned int length 
		* @example std::clog << tostr(25, 4) << std::endl; 0025
		* @example std::clog << tostr(25, 3) << std::endl; 025
		* @return std::string
	**/
	
	template <class T>
	std::string tostr(T input_number, unsigned int length) {
        std::ostringstream outstringstream;

        outstringstream << input_number;
		
		std::string return_string = outstringstream.str();
		
		if (return_string.length() < length) {
			unsigned int required = length - return_string.length();
			std::string fill_zeros;
			
			for ( unsigned int i = 0; i < required; ++i) {
				fill_zeros += "0";
			}
			
			return_string = fill_zeros + return_string;
		}

        return return_string;
	}
}

#endif