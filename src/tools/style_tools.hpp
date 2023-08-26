#include <fstream>
#include <string>
#include <tools/tools.hpp>
#include "style/style.hpp"
#include "settings.hpp"

namespace Tools {
	std::list <Style> parse_css(std::string file, std::shared_ptr <Settings> s);
}