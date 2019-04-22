#ifndef WEB_TOOLS_HPP
#define WEB_TOOLS_HPP

#include <SFML/network.hpp>
#include <string>
#include <list>
#include <iostream>

namespace Tools {
	/**
		* get_webdata
		* Get data from website loads whole data into string. Have to parse later
		* @param website
		* @param site
		* @param parameters
		* @return string
	**/
	
	std::string get_webdata(std::string website, std::string site, std::list <std::string> params);
	std::string get_webdata(std::string website, std::string site);
}

#endif