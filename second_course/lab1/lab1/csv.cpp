#include "csv.h"

namespace csvDumper {

	std::multimap<unsigned, const char*> flipMap(const std::map<std::string, unsigned>& mapIn) {
		std::multimap<unsigned, const char*> result;

		for (auto pair : mapIn) {
			result.insert(std::pair<unsigned, const char*>(pair.second, mapIn.find(pair.first)->first.data()));
		}

		return result;

	}

	void Dumper::dump(std::string path, textHandler::Words& words) {
		std::ofstream out(path);

		try {
			if (!out.is_open()) {
				throw std::exception("Program couldn't create output file.");
			}
			else {
				out << "Content,Amount,Frequency Percentage\n";

				std::multimap<unsigned, const char*> sortedWords = flipMap(words.getMap());

				for (auto iter = sortedWords.rbegin(); iter != sortedWords.rend(); iter++) {
					out << iter->second << "," << iter->first << "," << (double)iter->first / words.getWordsCnt() * 100 << "%\n";
				}
			}
		} catch (const char* err) {
			std::cerr << err << std::endl;
		}

		out.close();
	}
}