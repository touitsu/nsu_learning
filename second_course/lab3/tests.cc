#include <gtest/gtest.h>
#include "game.h"
#include <cstdint>

TEST(GameOfLifeMapTest, MapDefaultConstructorTest) {
	gameOfLife::Map map;
	const std::vector<std::vector<int8_t>> ex = {{0}};

	ASSERT_EQ(map.getContent(), ex);
}

TEST(GameOfLifeMapTest, MapSpecifiedSizeConstructorTest) {
	gameOfLife::Map map(2);
	const std::vector<std::vector<int8_t>> ex = { {0, 0}, {0, 0} };

	ASSERT_EQ(map.getContent(), ex);

	ASSERT_THROW(gameOfLife::Map(INT32_MAX), std::invalid_argument);
}

TEST(GameOfLifeMapTest, MapSetTest) {
	gameOfLife::Map map(2);
	const std::vector<std::vector<int8_t>> ex = { {1, 1}, {1, 1} };
	const std::vector<std::vector<int8_t>> ex2 = { {1, 0}, {1, 1} };
	
	for (int32_t i = 0; i < 2; i++) {
		for (int32_t j = 0; j < 2; j++) {
			map.set(i, j);
		}
	}
	ASSERT_EQ(map.getContent(), ex);

	map.set(1, 0, 0);
	ASSERT_EQ(map.getContent(), ex2);

	ASSERT_THROW(map.set(100, 100), std::out_of_range);
}

TEST(GameOfLifeMapTest, MapGetTest) {
	gameOfLife::Map map(2);

	map.set(0, 0);

	ASSERT_EQ(map.get(0, 0), 1);

	ASSERT_EQ(map.get(1, 1), 0);
}

TEST(GameOfLifeMapTest, MapGetSizeTest) {
	ASSERT_EQ(gameOfLife::Map(2).getSize(), 2);

	ASSERT_EQ(gameOfLife::Map(4).getSize(), 4);
}

TEST(GameOfLifeMapTest, MapGetContentTest) {
	const std::vector<std::vector<int8_t>> ex = { {0, 0}, {0, 0} };

	ASSERT_EQ(gameOfLife::Map(2).getContent(), ex);
}

TEST(GameOfLifeGameTest, GameExplicitConstructorTest) {
	gameOfLife::Game game = gameOfLife::Game(2, std::string("3"), std::string("23"), std::string("Name"));
	const std::vector<std::vector<int8_t>> ex = { {0, 0}, {0, 0} };

	ASSERT_EQ(game.getMap().getContent(), ex);
	ASSERT_EQ(game.getBx(), std::string("3"));
	ASSERT_EQ(game.getSy(), std::string("23"));
	ASSERT_EQ(game.getName(), std::string("Name"));
}

TEST(GameOfLifeGameTest, GameCountNeighboursTest) {
	gameOfLife::Game game = gameOfLife::Game(2, std::string("3"), std::string("23"), std::string("Name"));

	ASSERT_EQ(game.countNeighbours(0, 0), 0);

	game.set(0, 0);

	ASSERT_EQ(game.countNeighbours(0, 0), 0);
	ASSERT_EQ(game.countNeighbours(1, 0), 2);
}

TEST(GameOfLifeGameTest, GameCalculateNeighboursMapTest) {
	gameOfLife::Game game = gameOfLife::Game(2, std::string("3"), std::string("23"), std::string("Name"));
	const std::vector<std::vector<int8_t>> ex = { {0, 0}, {0, 0} };
	const std::vector<std::vector<int8_t>> ex2 = { {0, 2}, {2, 4} };

	game.calculateNeighboursMap();

	ASSERT_EQ(game.getNeighboursMap().getContent(), ex);

	game.set(0, 0);
	game.calculateNeighboursMap();

	ASSERT_EQ(game.getNeighboursMap().getContent(), ex2);
}

TEST(GameOfLifeGameTest, GameIterateTest) {
	gameOfLife::Game game = gameOfLife::Game(5, std::string("3"), std::string("23"), std::string("Name"));
	const std::vector<std::vector<int8_t>> ex = { {0, 0, 0, 0, 0}, {0, 0, 1, 0, 0}, {0, 0, 1, 0, 0}, {0, 0, 1, 0, 0}, {0, 0, 0, 0, 0} };
	
	game.set(1, 2);
	game.set(2, 2);
	game.set(3, 2);

	game.iterate();

	ASSERT_EQ(game.getMap().getContent(), ex);

	for (int32_t i = 0; i < 10; i++) {
		game.iterate();
	}

	ASSERT_EQ(game.getMap().getContent(), ex);
}

TEST(GameOfLifeGameTest, GameSetTest) {
	gameOfLife::Game game = gameOfLife::Game(5, std::string("3"), std::string("23"), std::string("Name"));
	const std::vector<std::vector<int8_t>> ex = { {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 1, 1, 1, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0} };

	game.set(1, 2);
	game.set(2, 2);
	game.set(3, 2);

	ASSERT_EQ(game.getMap().getContent(), ex);
}

TEST(GameOfLifeGameTest, GameGetMapTest) {
	gameOfLife::Game game = gameOfLife::Game(5, std::string("3"), std::string("23"), std::string("Name"));
	const std::vector<std::vector<int8_t>> ex = { {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0} };

	ASSERT_EQ(game.getMap().getContent(), ex);
}

TEST(GameOfLifeGameTest, GameGetNeighboursMapTest) {
	gameOfLife::Game game = gameOfLife::Game(2, std::string("3"), std::string("23"), std::string("Name"));
	const std::vector<std::vector<int8_t>> ex = { {0, 0}, {0, 0} };
	const std::vector<std::vector<int8_t>> ex1 = { {0, 2}, {2, 4} };

	game.calculateNeighboursMap();

	ASSERT_EQ(game.getNeighboursMap().getContent(), ex);

	game.set(0, 0);

	game.calculateNeighboursMap();

	ASSERT_EQ(game.getNeighboursMap().getContent(), ex1);
}

TEST(GameOfLifeGameTest, GameGetBxTest) {
	gameOfLife::Game game = gameOfLife::Game(2, std::string("3"), std::string("23"), std::string("Name"));

	ASSERT_EQ(game.getBx(), std::string("3"));
}

TEST(GameOfLifeGameTest, GameGetSyTest) {
	gameOfLife::Game game = gameOfLife::Game(2, std::string("3"), std::string("23"), std::string("Name"));

	ASSERT_EQ(game.getSy(), std::string("23"));
}

TEST(GameOfLifeGameTest, GameGetNameTest) {
	gameOfLife::Game game = gameOfLife::Game(2, std::string("3"), std::string("23"), std::string("Name"));

	ASSERT_EQ(game.getName(), std::string("Name"));
}