#ifndef _TIME_TOOLS_HPP
#define _TIME_TOOLS_HPP

#include "text_tools.hpp"

namespace Tools {
	/**
		* totime
		* converts double to time with specific format
		* @param double milliseconds
		* @param std::string milliseconds
		* @description
		* Format follows PHP's similar functions. 
	**/
		
	std::string totime(double milliseconds, std::string format = "H:i:s");
}

#endif
