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

			std::map<std::string, unsigned> getMap();

			int getWordsCnt();

			void readFromFileBySeparators(std::string& filePath, std::string separators);

	};
}
