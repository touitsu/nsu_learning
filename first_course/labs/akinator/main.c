#define _CRT_SECURE_NO_DEPRECATE
#include "functions.h"
#include <stdint.h>


int32_t main() {
	Tree* tree;
	FILE* file;

	file = fopen("data.txt", "r");

	tree = buildfromfile(file);

	fclose(file);

	printf("Welcome to Dota 2 Akinator. This programm can guess any character from Dota 2.\n");
	printf("Answer questions with yes or no.\n");

	playgame(tree);

	file = fopen("data.txt", "w");

	dumptofile(tree, file);

	fclose(file);

	freetree(tree);
}