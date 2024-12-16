#include "lab4.h"

using namespace AudioProcessor;

int main(int32_t argc, char** argv) {
	try {
		std::string configPath;
		std::string outputPath;
		std::shared_ptr<std::string> tmp;
		std::vector<std::shared_ptr<std::string>> inputs;


		if (argc < 5) {
			throw std::invalid_argument("Not enough arguments provided.\n");
		}

		else {
			for (size_t i = 0; i < argc; i++) {
				if (std::string(argv[i]) == std::string("-c")) {
					if (i + 1 >= argc) {
						throw std::invalid_argument("Path to config isn't provided.\n");
					}

					configPath.assign(argv[i + 1]);

					if (configPath.find(".txt") == std::string::npos) {
						throw std::invalid_argument("Invalid path config path, must be a txt file.\n");
					}

					if (i + 2 >= argc) {
						throw std::invalid_argument("Path to save isn't provided.\n");
					}

					outputPath.assign(argv[i + 2]);

					if (outputPath.find(".wav") == std::string::npos) {
						throw std::invalid_argument("Invalid output path, must be a wave file.\n");
					}

					if (i + 2 >= argc) {
						throw std::invalid_argument("Path to input isn't provided.\n");
					}

					for (size_t j = i + 3; j < argc; j++) {
						tmp = std::make_shared<std::string>(argv[j]);

						if (tmp.get()->find(".wav") == std::string::npos) {
							throw std::invalid_argument("Invalid input path, must be a wave file.\n");
						}

						inputs.push_back(tmp);
					}
				}
			}
		}

		ConfigHandler configHandler(outputPath, configPath, inputs);

		configHandler.handleConfig();

	}
	catch (std::invalid_argument err) {
		std::cerr << err.what() << std::endl;
		return 1;
	}
	catch (std::runtime_error err) {
		std::cerr << err.what() << std::endl;
		return 2;
	}
	return 0;
}
