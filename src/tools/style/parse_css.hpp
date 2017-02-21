#ifndef PARSE_CSS
#define PARSE_CSS

#include <fstream>
#include <iostream>

#include <list>

#include "style.hpp"
#include "../tools.hpp"

/**
	* Parse_css
	* This is bit overkill, but this namespace loops through css-files and parses list of items
**/

namespace parse_css {
	/**
		* parse
		* This parses given file. File must be well formed
		* @param std::sring file
		* @return std::list <Style>
	**/
	std::list <Style> parse(std::string file);
}

#endif