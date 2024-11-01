#pragma once

#include <vector>
#include <regex>
#include <fstream>

namespace gameOfLife {

	class Map {
	private:
		std::vector<std::vector<int8_t>> content;
	public:

		Map() {
			this->content.push_back(std::vector<int8_t>());
			this->content[0].push_back(0);
		}

		Map(const Map& map) {

			this->content.reserve(map.getSize());

			for (int32_t i = 0; i < map.getSize(); i++) {
				this->content.push_back(std::vector<int8_t>());
				this->content[i].reserve(map.getSize());
				for (int32_t j = 0; j < map.getSize(); j++) {
					this->content[i].push_back(map.get(j, i));
				}
			}
		}

		Map(int32_t x, int32_t y) {
			if (x > this->content.max_size()) {
				throw std::invalid_argument("Can't create a map of such size.");
			}

			this->content.reserve(x);
			for (int32_t i = 0; i < y; i++) {
				this->content.push_back(std::vector<int8_t>());
				if (y > this->content[i].max_size()) {
					throw std::invalid_argument("Can't create a map of such size.");
				}
				this->content[i].reserve(y);
				for (int32_t j = 0; j < x; j++) {
					this->content[i].push_back(0);
				}
			}
		}

		void set(int32_t x, int32_t y, int8_t val = 1) {
			if (y >= this->content.size() || x >= this->content.size() || x < 0 || y < 0) {
				throw std::out_of_range("Out of map range.\n");
			}
			(this->content)[y][x] = val;
		}

		int8_t get(int32_t x, int32_t y) const noexcept {
			return (this->content)[y][x];
		}

		size_t getSize() const noexcept {
			return this->content.size();
		}

		const std::vector<std::vector<int8_t>>& getContent() const noexcept {
			return this->content;
		}
	};

	class Game {
	private:
		Map map;
		Map neighboursMap;
		std::string Bx;
		std::string Sy;
		std::string Name;

	public:
		explicit Game(int32_t x, int32_t y, std::string& Bx, std::string& Sy, std::string& name) {
			if (x <= 0) {
				throw std::invalid_argument("X must be positive and bigger than zero.");
			}
			if (y <= 0) {
				throw std::invalid_argument("Y must be positive and bigger than zero.");
			}

			map = Map(x, y);
			neighboursMap = Map(x, y);

			std::regex bxRegex("^[0-8]");
			std::regex syRegex("^[0-8]{1,8}");

			if (!std::regex_match(Bx, bxRegex)) {
				throw std::invalid_argument("Bx must be between 0 and 8.");
			}

			if (!std::regex_match(Sy, syRegex)) {
				throw std::invalid_argument("Sy must be between 0 and 8.");
			}

			this->Bx = Bx;
			this->Sy = Sy;
			this->Name = name;
		}

		int8_t countNeighbours(int32_t x, int32_t y) const noexcept {
			int8_t res;

			res = 0;


			for (int32_t i = x - 1; i <= x + 1; i++) {
				for (int32_t j = y - 1; j <= y + 1; j++) {
					res += this->map.get((this->map.getSize() + i) % this->map.getSize(), (this->map.getSize() + j) % this->map.getSize());
				}
			}

			return res - this->getMap().get(x, y);
		}

		void calculateNeighboursMap() noexcept {
			for (int32_t i = 0; i < this->map.getSize(); i++) {
				for (int32_t j = 0; j < this->map.getSize(); j++) {
					this->neighboursMap.set(i, j, countNeighbours(i, j));
				}
			}
		}

		void iterate() noexcept {

			calculateNeighboursMap();

			for (int32_t x = 0; x < this->map.getSize(); x++) {
				for (int32_t y = 0; y < this->map.getSize(); y++) {
					if (this->map.get(x, y) == 0) {
						if (std::stoi(Bx) == this->neighboursMap.get(x, y)) {
							this->map.set(x, y, 1);
						}
					}
					else {
						if (this->map.get(x, y) == 1 && Sy.find(this->neighboursMap.get(x, y) + '0') == std::string::npos) {
							this->map.set(x, y, 0);
						}
					}
				}
			}
		}

		~Game() {

		}

		void set(int32_t x, int32_t y, int8_t val = 1) noexcept {
			this->map.set(x, y, val);
		}

		const Map& getMap() const noexcept {
			return this->map;
		}

		void printMap() const noexcept{
			for (auto line : this->getMap().getContent()) {
				for (auto elem : line) {
					std::cout << (bool)elem << " ";
				}
				std::cout << std::endl;
			}
		}

		const std::string& getBx() const noexcept {
			return this->Bx;
		}

		const std::string& getSy() const noexcept {
			return this->Sy;
		}

		const std::string& getName() const noexcept {
			return this->Name;
		}

		void setName(std::string& name) noexcept {
			this->Name = name;
		}

	};

	class CommandHandler {
	private:
		std::string currentCommand;
	public:
		void getCommand() {
			while (currentCommand.empty()) {
				std::getline(std::cin, currentCommand);
			}
		}

		void getAndHandleCommand(std::vector<bool>& flags, int32_t& iterations, std::string& returnString) {
			this->getCommand();

			static std::regex dumpRegex("^[dD]([uU][mM][Pp])? [A-z0-9.]*");
			static std::regex iterRegex("^[tT]([iI][cC][kK])?[0-9 ]*");
			static std::regex exitRegex("^[eE]([xX][iI][tT])?");
			static std::regex helpRegex("^[hH]([eE][lL][pP])?");
			static std::regex nameRegex("^[nN]([aA][mM][eE])? [A-z0-9.]*");

			if (std::regex_match(currentCommand, dumpRegex)) {
				flags[0] = 1;
				returnString = currentCommand.substr(currentCommand.find(' ') + 1, currentCommand.length() - currentCommand.find(' ') - 1);
			}

			else if (std::regex_match(currentCommand, iterRegex)) {
				flags[1] = 1;
				iterations = 0;
				for (char c : currentCommand) {
					if (c >= '0' && c <= '9') {
						iterations *= 10;
						iterations += c - '0';
					}
				}
				if (iterations == 0) {
					iterations = 1;
				}
			}

			else if (std::regex_match(currentCommand, exitRegex)) {
				flags[2] = 1;
			}

			else if (std::regex_match(currentCommand, helpRegex)) {
				flags[3] = 1;
			}

			else if (std::regex_match(currentCommand, nameRegex)) {
				flags[5] = 1;
				returnString = currentCommand.substr(currentCommand.find(' ') + 1, currentCommand.length() - currentCommand.find(' ') - 1);
			}

			else {
				flags[4] = 1;
			}

			this->currentCommand.clear();
		}
	};

	class Dumper {
	private:
		std::string& path;
		std::ofstream file;
	public:
		Dumper(std::string& path) : path(path) {
			this->file.open(this->path);

			if (!this->file.is_open()) {
				throw std::exception("Can not open file to read.\n");
			}
		}

		~Dumper() {
			this->file.close();
		}

		void Dump(Game& game) {
			std::string tmp;
				
			file << "#Life 1.06\n";

			file << "#N " << game.getName() << "\n";

			file << "#R B" << game.getBx() << '/';

			file << 'S' << game.getSy() << "\n";
				
			for (int32_t x = 0; x < game.getMap().getSize(); x++) {
				for (int32_t y = 0; y < game.getMap().getSize(); y++) {
					if (game.getMap().get(x, y)) {
						file << x << " " << y << "\n";
					}
				}
			}

			std::cout << "Game " << game.getName() << " succesfully dumped.\n";
		}
	};

	class Loader {
	private:
		std::string& path;
		std::ifstream file;
	public:
		Loader(std::string& path) : path(path) {
			this->file.open(path);

			if (!file.is_open()) {
				throw std::exception("Can not open file to read.\n");
			}
		}

		~Loader() {
			file.close();
		}

		void load(Game** game) {
			int32_t minX, minY, maxX, maxY, curX, curY, idx;
			std::string line;
			std::string name;
			bool rulesSpecified;
			static std::regex firstLineRegex("^#Life 1.06");
			static std::regex secondLineRegex("^#N [A-Za-z.1-9]*");
			static std::regex thirdLineRegex("^#R B[1-9]/S[1-9]*");

			rulesSpecified = true;
			idx = 3;
			minX = INT32_MAX;
			minY = INT32_MAX;
			maxX = INT32_MIN;
			maxY = INT32_MIN;

			std::getline(file, line);
			if (!std::regex_match(line, firstLineRegex)) {
				throw std::exception("File is not in the Life1.06 format, exception occured on the line 0, is header there?\n");
			}

			std::getline(file, line);
			if (!std::regex_match(line, secondLineRegex)) {
				throw std::exception("File is not in the Life1.06 format, exception occured on the line 1, does your universe have a name?\n");
			}

			std::getline(file, line);
			if (!std::regex_match(line, thirdLineRegex)) {
				std::cout << "There is no rules specified in the input file, game will be played with default rules.\n";
				rulesSpecified = false;
			}

			std::getline(file, line);

			while (file) {
				if (line.find(' ') == line.npos) {
					line = "File is not in the Life1.06 format, exception occured on the line ";
					line += std::to_string(idx);
					line += " is there a space between coordinates?\n";
					throw std::exception(line.c_str());
				}

				curX = atoi(line.substr(0, line.find(' ')).c_str());
				curY = atoi(line.substr(line.find(' '), line.length() - line.find(' ')).c_str());

				minX = min(minX, curX);
				minY = min(minY, curY);
				maxX = max(maxX, curX);
				maxY = max(maxY, curY);

				idx++;

				std::getline(file, line);
			}

			file.close();
			file.open(path);

			std::getline(file, line);
			std::getline(file, line);

			name = line.substr(line.find(' ') + 1, line.length() - line.find(' ') - 1);

			if (rulesSpecified) {
				std::getline(file, line);

				*game = new Game(max((maxX - minX + 1) * 2, (maxY - minY + 1) * 2) + 1, 
								max((maxX - minX + 1) * 2, (maxY - minY + 1) * 2) + 1, 
								line.substr(line.find("B") + 1, 1), 
								line.substr(line.find("S") + 1, line.length() - line.find("S") - 1), name);
			}
			else {
				*game = new Game(max((maxX - minX + 1) * 2, (maxY - minY + 1) * 2) + 1, 
								max((maxX - minX + 1) * 2, (maxY - minY + 1) * 2) + 1, 
								std::string("3"), std::string("23"), name);
			}
				
			std::getline(file, line);

			while (file) {
				curX = atoi(line.substr(0, line.find(' ')).c_str());
				curY = atoi(line.substr(line.find(' '), line.length() - line.find(' ')).c_str());

				(*game)->set(max((maxX - minX + 1), (maxY - minY + 1)) - (minX + maxX)/2 + curX, 
							max((maxX - minX + 1), (maxY - minY + 1)) - (minY + maxY) / 2 + curY);

				std::getline(file, line);
			}

			std::cout << "Game " << name << " loaded succesfully\n";
		}
	};
}
