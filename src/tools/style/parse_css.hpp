#include <fstream>
#include <iostream>

#include <list>

#include "../../views/layout/style.hpp"
#include "../tools.hpp"

namespace parse_css {
	std::list <Style> parse(std::string file);
}
