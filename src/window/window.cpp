#include "window.hpp"

void Window::load_settings() {
	#ifdef DEBUG
	std::clog << "Window::load_settings()" << std::endl;
	#endif
	
	std::shared_ptr <Settings> s (new Settings);
	this->settings = s;
	
	Tools::init_random();
	std::map <std::string, std::string> tmp;
	std::string line;
	//std::string type;
	std::string name;
	std::string value;
	
	std::ifstream setting_file("settings/settings.ini", std::ios::in);
	
	while (std::getline(setting_file, line)) {
		std::vector <std::string> cells = Tools::split(" ", line);
		
		if (cells.size() == 3) {
			//type = cells[0];
			name = cells[1];
			value = cells[2];
			
			tmp[name] = value;
		}
	}
	
	setting_file.close();

	this->settings->set_values(tmp);
}