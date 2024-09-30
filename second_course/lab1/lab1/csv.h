#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include "text.h"

namespace csvDumper {

	class Dumper {

		public:
			void dump(std::string path, textHandler::Words& words);

	};
}