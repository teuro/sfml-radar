#include "parse_css.hpp"

std::list <Style> parse_css::parse(std::string file) {
	std::ifstream fin(file.c_str(), std::ios::in);
    std::string line;
    std::string id;
    std::string s_class;
    std::string name;

    std::list <Style> tmp;

    while (std::getline(fin, line)) {
		size_t found = line.find("{");
        if (found != std::string::npos) {
			Style t;
			
			if (line.substr(0, 1) == "#") {
				id = line.substr(1, found-1);
				id = Tools::trim(id);
				t.set_attribute("id", id);
			} else if (line.substr(0, 1) == ".") {
				s_class = line.substr(1, found-1);
				s_class = Tools::trim(s_class);
				t.set_attribute("class", s_class);
			} else {
				name = line.substr(0, found-1);
				name = Tools::trim(name);
				t.set_attribute("name", name);
			}
			
			tmp.push_back(t);
        } else if (line.substr(0, 1) != "}" && line != "") {
            std::string key     = Tools::trim(line.substr(0, line.find(":")));
            std::string value   = Tools::trim(line.substr(line.find(":")+1));

            tmp.back().set_attribute(key, value);
		}
    }

    fin.close();

    return tmp;
}
