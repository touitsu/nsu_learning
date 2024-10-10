#include "csv.h"

namespace csvDumper {

	void Dumper::dump(std::string path, textHandler::Words& words) {
		std::ofstream out(path);

		try {
			if (!out.is_open()) {
				throw std::exception("Program couldn't create output file.");
			}
			else {
				out << "Content,Amount,Frequency Percentage\n";

				std::multimap<unsigned, std::string, std::greater<unsigned>> sortedWords = words.getMap();

				for (auto iter = sortedWords.begin(); iter != sortedWords.end(); iter++) {
					std::cout << iter->second << "," << iter->first << "," << (double)iter->first / words.getWordsCnt() * 100 << "%\n";
					out << iter->second << "," << iter->first << "," << (double)iter->first / words.getWordsCnt() * 100 << "%\n";
				}
			}
		} catch (const char* err) {
			std::cerr << err << std::endl;
		}

		out.close();
	}
}