#define _CRT_SECURE_NO_DEPRECATE
#include "functions.h"
#include <stdint.h>


int32_t main() {
	Tree* tree;
	FILE* file;

	file = fopen("data.txt", "r");

	tree = buildfromfile(file);

	fclose(file);

	playgame(tree);

	file = fopen("data.txt", "w");

	dumptofile(tree, file);

	fclose(file);

	freetree(tree);
}