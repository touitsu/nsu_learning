#include "csv.h"

namespace csvDumper {

	std::pair<unsigned, std::string> flipPair(const std::pair<std::string, unsigned>& pairIn) {
		return std::pair<unsigned, std::string>(pairIn.second, pairIn.first);
	}

	std::multimap<unsigned, std::string> flipMap(const std::map<std::string, unsigned>& mapIn) {
		std::multimap<unsigned, std::string> result;

		std::transform(mapIn.begin(), mapIn.end(), std::inserter(result, result.begin()), flipPair);

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

				std::multimap<unsigned, std::string> sortedWords = flipMap(words.getMap());

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