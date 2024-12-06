#include <iostream>
#include <cstdint>
#include <Windows.h>
#include "game.h"

int32_t main(int32_t argc, char* argv[]) {
	gameOfLife::Game* game;
	gameOfLife::CommandHandler commandHandler;
	
	std::string returnString;
	bool offline = false;
	int32_t iterations;

	iterations = INT32_MIN;

	gameOfLife::Operation operation = gameOfLife::Operation::help;

	if (argc == 1) {
		game = new gameOfLife::Game(21, std::string("3"), std::string("23"), std::string("name"));

		//game->set(5, 4, 1);
		//game->set(5, 5, 1);
		//game->set(5, 6, 1);
		//game->set(4, 5, 1);
		//game->set(6, 6, 1);

		game->set(6, 4, 1);
		game->set(7, 4, 1);
		game->set(8, 4, 1);
		game->set(12, 4, 1);
		game->set(13, 4, 1);
		game->set(14, 4, 1);
		game->set(4, 6, 1);
		game->set(9, 6, 1);
		game->set(11, 6, 1);
		game->set(16, 6, 1);
		game->set(4, 7, 1);
		game->set(9, 7, 1);
		game->set(11, 7, 1);
		game->set(16, 7, 1);
		game->set(4, 8, 1);
		game->set(9, 8, 1);
		game->set(11, 8, 1);
		game->set(16, 8, 1);
		game->set(6, 9, 1);
		game->set(7, 9, 1);
		game->set(8, 9, 1);
		game->set(12, 9, 1);
		game->set(13, 9, 1);
		game->set(14, 9, 1);
		game->set(6, 11, 1);
		game->set(7, 11, 1);
		game->set(8, 11, 1);
		game->set(12, 11, 1);
		game->set(13, 11, 1);
		game->set(14, 11, 1);
		game->set(4, 12, 1);
		game->set(9, 12, 1);
		game->set(11, 12, 1);
		game->set(16, 12, 1);
		game->set(4, 13, 1);
		game->set(9, 13, 1);
		game->set(11, 13, 1);
		game->set(16, 13, 1);
		game->set(4, 14, 1);
		game->set(9, 14, 1);
		game->set(11, 14, 1);
		game->set(16, 14, 1);
		game->set(6, 16, 1);
		game->set(7, 16, 1);
		game->set(8, 16, 1);
		game->set(12, 16, 1);
		game->set(13, 16, 1);
		game->set(14, 16, 1);

		game->printMap();
	}
	else if (argc == 2) {
		std::string tmp(argv[1]);
		gameOfLife::Loader loader(tmp);

		loader.load(&game);

		game->printMap();
	}
	else {
		offline = true;
		for (int32_t i = 0; i < argc; i++) {
			if (std::string(argv[i]) == std::string("-i") && i != argc - 1) {
				iterations = atoi(argv[i + 1]);
			}
		}
		for (int32_t i = 0; i < argc; i++) {
			if (std::string(argv[i]) == std::string("-o") && i != argc - 1) {
				returnString = argv[i+1];
			}
		}
		
		if (iterations == INT32_MIN) {
			throw std::exception("Invalid arguments.");
		}

		if (returnString.empty()) {
			throw std::exception("Invalid arguments.");
		}

		std::string tmp(argv[1]);
		gameOfLife::Loader loader(tmp);

		loader.load(&game);
	}

	while (operation != gameOfLife::Operation::exit && !offline) {

		commandHandler.getAndHandleCommand(operation, iterations, returnString);

		system("cls");
		game->printMap();

		switch (operation) {

			case gameOfLife::Operation::dump: {
				gameOfLife::Dumper dumper(returnString);
				try {
					dumper.Dump(*game);
				}
				catch (std::exception& ex) {
					std::cout << ex.what();
				}
				break;
			}

			case gameOfLife::Operation::iterate:
				system("cls");
				for (int32_t i = 0; i < iterations; i++) {
					game->iterate();
				}
				game->printMap();
				std::cout << "Successfully iterated " << iterations << " times.\n";
				iterations = 0;
				break;

			case gameOfLife::Operation::exit:
				std::cout << "Exciting...\n";
				break;

			case gameOfLife::Operation::help:
				std::cout << "List of commands:\n";
				std::cout << "1. dump <filename> - dumps your game at the current state to the <filename> (short version - d <filename>).\n";
				std::cout << "2. tick <n=1> - iterates your game for given ammount of ticks (short version - t <n=1>).\n";
				std::cout << "3. exit - exits the game safely (short version - e).\n";
				std::cout << "4. name <yourname> - allows you to name your game (must follow regex for valid name)(short version - n).\n";
				std::cout << "5. help - gives you this list of commands (short version - h).\n";
				break;

			case gameOfLife::Operation::name:
				game->setName(returnString);
				std::cout << "Renamed your game to " << returnString << "\n";
				break;

			case gameOfLife::Operation::unknownCommand:
				std::cout << "Unavaliable command, consider using \"help\" command.\n";
				break;

		}
	}

	if (offline) {
		for (int32_t i = 0; i < iterations; i++) {
			game->iterate();
		}
		gameOfLife::Dumper dumper(returnString);
		dumper.Dump(*game);
	}

	delete game;

	return 0;
}
