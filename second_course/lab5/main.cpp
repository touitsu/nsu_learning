#include "csvParser.h"

int32_t main() {
	try {
		std::ifstream file("in.csv");

		Config conf = Config();

		if (!file.is_open()) {
			throw InvalidArgumentException(std::string("main"), std::string("Failed to open file."));
		}

		csvParser<std::string, int32_t> parser(file, 0, conf);

		for (auto rs : parser) {
			std::cout << rs << std::endl;
		}
	}
	catch (Exception& err) {
		std::cout << err.info() << std::endl;
	}

	return 0;
}
