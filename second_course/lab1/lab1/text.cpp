#include "text.h"

namespace textHandler {

	Words::Words() {
		wordsMap = std::multimap<unsigned, std::string, std::greater<unsigned>>();
		wordsCnt = 0;
	}

	const std::multimap<unsigned, std::string, std::greater<unsigned>>& Words::getMap() const{
		return this->wordsMap;
	}

	const int& Words::getWordsCnt() const{
		return this->wordsCnt;
	}

	void Words::readFromFileBySeparators(std::string& filePath, std::string separators) {
		std::string line, tmpStr;
		std::ifstream in;

		try {
			in = std::ifstream(filePath);

			if (!in.is_open()) {
				throw std::exception("Programm is unable to open the file.\n(Does your file exist?)");
			}

			while (in) {
				
				std::getline(in, line);

				for (int i = 0; i < line.size(); i++) {
					if (separators.find(line[i]) == std::string::npos) {
						tmpStr.push_back(line[i]);
					}

					else {
						for (auto p = wordsMap.begin(); p != wordsMap.end(); p++) {
							if (p->second == tmpStr) {
								wordsMap.insert(std::make_pair(p->first + 1, p->second));
								p = wordsMap.erase(p);
								wordsCnt++;
								tmpStr.clear();
								if (p == wordsMap.end()) {
									break;
								}
							}
						}
						
						if (!tmpStr.empty()) {
							wordsMap.insert(std::make_pair(1, tmpStr));
							wordsCnt++;
							tmpStr.clear();
						}
					}
				}
			}

		} catch (const std::exception& ex) {
			std::cerr << ex.what() << std::endl;
		}
	}
}
