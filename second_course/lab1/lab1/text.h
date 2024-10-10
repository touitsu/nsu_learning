#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>

namespace textHandler {

	class Words {
		private:
			std::multimap<unsigned, std::string, std::greater<unsigned>> wordsMap;
			int wordsCnt;

		public:

			Words();

			const std::multimap<unsigned, std::string, std::greater<unsigned>>& getMap() const;

			const int& getWordsCnt() const;

			void readFromFileBySeparators(std::string& filePath, std::string separators);

	};
}
