#include "AudioProcessor.h"
#include "exception.h"

using namespace AudioProcessor;

int main(int32_t argc, char** argv) {
	try {
		std::string configPath;
		std::string outputPath;
		std::shared_ptr<std::string> tmp;
		std::vector<std::shared_ptr<std::string>> inputs;


		if (argc < 5 && std::string(argv[1]) != std::string("-h")) {
			throw std::invalid_argument("Not enough arguments provided, consider using just a -h flag.\n");
		}

		else {
			for (size_t i = 0; i < argc; i++) {
				if (std::string(argv[i]) == std::string("-h")) {
					std::cout << "To use the programm write a config file config.txt inside the directory of the executable,\n"
						<< "then specify processors that you want to apply.\n"
						<< "For now there are 4 processors:\n"
						<< "mute start_seconds (end_seconds), mutes your first inputed audio file in the given interval, end_seconds are optional.\n"
						<< "mix second_input start_seconds (end_seconds), second_input is specified with dollar sign and an integer (e.g. $2 - second input), end_seconds are once again optional.\n"
						<< "echo start_seconds (end_seconds), mixes the first input with itself with start_seconds as delta, end_seconds are optional\n"
						<< "save , saves output to output path, optional, programm saves your file automaticly.\n"
						<< "config files also support comments, which start with new line and #.\n";
				}

				if (std::string(argv[i]) == std::string("-c")) {
					if (i + 1 >= argc) {
						throw Exception(std::string("main"), std::string("Invalid argument"), std::string("Path to config isn't provided."));
					}

					configPath.assign(argv[i + 1]);

					if (configPath.find(".txt") == std::string::npos) {
						throw Exception(std::string("main"), std::string("Invalid argument"), std::string("Invalid config path, must be a txt file."));
					}

					if (i + 2 >= argc) {
						throw Exception(std::string("main"), std::string("Invalid argument"), std::string("Path to save isn't provided."));
					}

					outputPath.assign(argv[i + 2]);

					if (outputPath.find(".wav") == std::string::npos) {
						throw Exception(std::string("main"), std::string("Invalid argument"), std::string("Invalid output path, must be a wave file."));
					}

					if (i + 2 >= argc) {
						throw Exception(std::string("main"), std::string("Invalid argument"), std::string("Path to input isn't provided."));
					}
					for (size_t j = i + 3; j < argc; j++) {
						tmp = std::make_shared<std::string>(argv[j]);

						if (tmp.get()->find(".wav") == std::string::npos) {
							throw Exception(std::string("main"), std::string("Invalid argument"), std::string("Invalid input path, must be a wave file."));
						}

						inputs.push_back(tmp);
					}
				}
			}
		}

		if (configPath.size() != 0) {
			ConfigHandler configHandler(outputPath, configPath, inputs);

			configHandler.handleConfig();
		}
	
	}
	catch (Exception& err) {
		std::cerr << err.info() << std::endl;
		return 1;
	}

	return 0;
}
