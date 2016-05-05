#include "parse_css.hpp"

std::list <Style> parse_css::parse(std::string file) {
    std::ifstream fin(file.c_str(), std::ios::in);
    std::string line;
    std::string id;

    std::list <Style> tmp;

    while (std::getline(fin, line)) {
		size_t found = line.find("{");
        if (found != std::string::npos) {
			if (line.substr(0, 1) == "#" || line.substr(0, 1) == ".") {
				id = line.substr(1, found-1);
			} else {
				id = line.substr(0, found-1);
			}
			id = Tools::trim(id);
            Style t(id);
			//std::clog << "nimi on " << id << std::endl;

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
