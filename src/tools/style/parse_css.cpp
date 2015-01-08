#include "parse_css.hpp"

std::list <Style> parse(std::string file) {
    std::ifstream fin(file.c_str(), std::ios::in);
    std::string line;
    std::string id;

    std::list <Style> tmp;

    while (std::getline(fin, line)) {
        if (line.substr(0, 1) == "#" || line.substr(0, 1) == ".") {
            id = line.substr(1, line.find(" ")-1);
            Style t(id);

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
