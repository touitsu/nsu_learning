#pragma once

#include <iostream>
#include <vector>
#include <regex>
#include <fstream>

namespace gameOfLife {

	class Map {
	private:
		std::vector<std::vector<int8_t>> content;
	public:

		Map();

		Map(const Map& map);

		Map(int32_t x, int32_t y);

		void set(int32_t x, int32_t y, int8_t val = 1);

		int8_t get(int32_t x, int32_t y) const noexcept;

		size_t getSize() const noexcept;

		const std::vector<std::vector<int8_t>>& getContent() const noexcept;
	};

	class Game {
	private:
		Map map;
		Map neighboursMap;
		std::string Bx;
		std::string Sy;
		std::string Name;
		int32_t intBx;

	public:
		explicit Game(int32_t x, int32_t y, std::string& Bx, std::string& Sy, std::string& name);

		int8_t countNeighbours(int32_t x, int32_t y) const noexcept;

		void calculateNeighboursMap() noexcept;

		void iterate() noexcept;

		~Game();

		void set(int32_t x, int32_t y, int8_t val = 1) noexcept;

		const Map& getMap() const noexcept;

		void printMap() const noexcept;

		const std::string& getBx() const noexcept;

		const std::string& getSy() const noexcept;

		const std::string& getName() const noexcept;

		void setName(std::string& name) noexcept;

	};

	enum class Operation {
		dump,
		iterate,
		exit,
		help,
		unknownCommand,
		name
	};

	class CommandHandler {
	private:
		std::string currentCommand;
	public:
		void getCommand();

		void getAndHandleCommand(Operation& operation, int32_t& iterations, std::string& returnString);
	};

	class Dumper {
	private:
		std::string& path;
		std::ofstream file;
	public:
		Dumper(std::string& path);

		~Dumper();

		void Dump(Game& game);
	};

	class Loader {
	private:
		std::string& path;
		std::ifstream file;
	public:
		Loader(std::string& path);

		~Loader();

		void load(Game** game);
	};
}
