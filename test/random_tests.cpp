#include "../src/tools/tools.hpp"

#include <fstream>

int main(int argc, char** argv) {
	std::ofstream out("random_data.txt", std::ios::out);
	
	Tools::init_random();
	
	double arvo;
	
	for (int i = 0; i < 500; ++i){
		out << Tools::round_nearest(Tools::normal_distribution(220, 40), 5) << " " << Tools::linear_random(50, 85) << std::endl;
	}
	
	out.close();
}