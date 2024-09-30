#include "text.h"

namespace textHandler {

	Words::Words() {
		wordsMap = std::map<std::string, unsigned>();
		wordsCnt = 0;
	}

	std::map<std::string, unsigned> Words::getMap() {
		return this->wordsMap;
	}

	int Words::getWordsCnt() {
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
						if (wordsMap.find(tmpStr) != wordsMap.end()) {
							wordsMap.find(tmpStr)->second++;
							wordsCnt++;
						}
						
						else if (!tmpStr.empty()) {
							wordsMap[tmpStr] = 1;
							wordsCnt++;
						}

						tmpStr.clear();
					}
				}
			}

		} catch (const std::exception& ex) {
			std::cerr << ex.what() << std::endl;
		}
	}
}
