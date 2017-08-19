#include "time_tools.hpp"

std::string Tools::totime(double milliseconds, std::string format) {
	std::string time_string;
	
	int seconds = milliseconds / 1000;
	int hours = seconds / 3600;
	seconds -= hours * 3600;
	int minutes = seconds / 60;
	seconds -= minutes * 60;
	
	std::map <std::string, std::string> format_table;
	
	format_table["H"] = tostr(hours, 2);
	format_table["h"] = tostr(hours, 1);
	format_table["i"] = tostr(minutes, 2);
	format_table["m"] = tostr(minutes, 1);
	format_table["s"] = tostr(seconds, 2);
	format_table["t"] = tostr(seconds, 1);
	
	time_string = replace(format, format_table);
	
	return time_string;
}