#include <iostream>
#include "text.h"
#include "csv.h"

int main(int argc, char* argv[]) {
	textHandler::Words words;
	std::string path;
	unsigned totalAmountOfWords;
	csvDumper::Dumper dumper;


	try {
		if (argc == 3) {
			path = argv[1];

			words.readFromFileBySeparators(path, ". ,/\\|()[]-+=_*&\n^%$#@!?<>`{}:;\"'	");

			if (!words.getMap().empty()) {
				dumper.dump(argv[2], words);
			}
		}
		else {
			throw std::invalid_argument("Wrong arguments. (e.g. wordfreq.exe <yourtextfile> <nameofoutputfileincsvformat>)");
		}
	} catch (const std::exception& ex) {
		std::cerr << ex.what() << std::endl;
	}

	return 0;
}