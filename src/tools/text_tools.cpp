#include "text_tools.hpp"

bool Tools::is_match(std::string a, std::string b) {
	for (unsigned int i = 0; i < a.length(); ++i) {
		if (a[i] != b[i]) {
			return false;
		}
	}
	
	return true;
}

std::vector <std::string> Tools::split(std::string delimiter, std::string input) {
	if (input.length() == 0) {
		throw std::logic_error("input can't be empty");
	}

    std::vector <std::string> tmp_cells;
    while (true) {
        std::string::size_type pos = input.find(delimiter, 0);

        if (pos == std::string::npos) {
            tmp_cells.push_back(input);
            return tmp_cells;
        }
		
        tmp_cells.push_back(input.substr(0, pos));
        input.erase(0, pos + 1);
    }
}

std::string Tools::ltrim(std::string s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
    return s;
}

std::string Tools::rtrim(std::string s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
    return s;
}

std::string Tools::trim(std::string s) {
    return ltrim(rtrim(s));
}

std::string Tools::replace(std::string original, std::map <std::string, std::string> repl) {
	std::map <std::string, std::string> :: iterator it;
	
    for (it = repl.begin(); it != repl.end(); ++it) {
		original = replace(original, it->first, it->second);
    }
	
	return original;
}

std::string Tools::replace(std::string original, std::string find, std::string replace) {
	std::string::size_type pos = original.find(find, 0);
		
	if (pos != std::string::npos) {
		std::string part1 = original.substr(0, pos);
		std::string part2 = original.substr(pos, find.length());
		std::string part3 = original.substr(pos + find.length());
		
		part2 = replace;
		
		original = part1 + part2 + part3;
	}

	return original;
}