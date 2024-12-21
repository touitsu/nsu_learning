
#include "csvParser.h"

int32_t main() {
	std::ifstream file("in.csv");

	try {
		csvParser<std::string, int32_t> parser(file);

		for (auto rs : parser) {
			std::cout << rs << std::endl;
		}
	}
	catch (std::runtime_error err) {
		std::cout << err.what() << std::endl;
	}

	return 0;
}
