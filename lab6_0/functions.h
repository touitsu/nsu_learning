#pragma once


#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <malloc.h>

#ifndef FUNCTIONS
#define FUNCTIONS

typedef struct TREE {
	int32_t key;
	int16_t height;
	struct TREE* left;
	struct TREE* right;
}TREE;


TREE* inittree(TREE* tr, int32_t key);

int8_t getheight(TREE* tr);

int8_t bf(TREE* tr);

TREE* leftrot(TREE* tr);

TREE* rightrot(TREE* tr);

TREE* balance(TREE* tr);

TREE* insert(TREE* tr, int32_t key);

void symgothrough(TREE* tr);

TREE* search(TREE* tr, int32_t key);

void freetree(TREE* tr);

#endif