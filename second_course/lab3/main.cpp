#include <iostream>
#include <cstdint>
#include <Windows.h>
#include "game.h"

int32_t main(int32_t argc, char* argv[]) {
	gameOfLife::Game* game = new gameOfLife::Game(10, 10, std::string("3"), std::string("23"));
	gameOfLife::CommandHandler commandHandler;
	std::string dumpPath;
	std::vector<bool> flags;
	int32_t iterations;

	flags.reserve(5);
	flags = { 0, 0, 0, 0, 0 };

	//game->set(5, 4, 1);
	//game->set(5, 5, 1);
	//game->set(5, 6, 1);
	//game->set(4, 5, 1);
	//game->set(6, 6, 1);

	game->set(5, 4, 1);
	game->set(5, 5, 1);
	game->set(5, 3, 1);

	game->printMap();

	while (!flags[2]) {

		flags = { 0, 0, 0, 0, 0 };

		commandHandler.getAndHandleCommand(flags, iterations, dumpPath);

		system("cls");
		game->printMap();

		if (flags[0]) {
			if (!dumpPath.empty()) {
				gameOfLife::Dumper dumper(dumpPath);
				try {
					dumper.Dump(*game);
					std::cout << "Game succesfully dumped.\n";
				}
				catch (std::exception& ex) {
					std::cout << ex.what();
				}
				
			}
			else {
				std::cout << "Unavaliable file path.\n";
			}
		}
		else if (flags[1]) {
			system("cls");
			for (int32_t i = 0; i < iterations; i++) {
				game->iterate();
			}
			game->printMap();
			iterations = 0;
		}
		else if (flags[3]) {
			std::cout << "List of commands:\n";
			std::cout << "1. dump <filename> - dumps your game at the current state to the <filename> (short version - d <filename>).\n";
			std::cout << "2. tick <n=1> - iterates your game for given ammount of ticks (short version - t <n=1>).\n";
			std::cout << "3. exit - exits the game safely (short version - e).\n";
			std::cout << "4. help - gives you this list of commands (short version - h).\n";
		}
		else if (flags[4]) {
			std::cout << "Unavaliable command, consider using \"help\" command.\n";
		}
	}

	return 0;
}
