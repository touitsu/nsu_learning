#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>

namespace textHandler {

	class Words {
		private:
			std::map<std::string, unsigned> wordsMap;
			int wordsCnt;

		public:

			Words();

			const std::map<std::string, unsigned>& getMap() const;

			const int& getWordsCnt() const;

			void readFromFileBySeparators(std::string& filePath, std::string separators);

	};
}
