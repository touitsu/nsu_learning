#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <malloc.h>

#ifndef FUNCTIONS
#define FUNCTIONS


typedef struct string {
	char* content;
	int32_t len;
}string;

typedef struct Tree {
	int32_t key;
	string* value;
	struct Tree* right;
	struct Tree* left;
}Tree;


string* initstring();

Tree* inittree(int32_t key, string* content);

void freetree(Tree* tree);

void addtotree(Tree* tree, int32_t key, string* content);

void printstr(string* str);

void printtree(Tree* tree);

Tree* buildfromfile(FILE* file);

string* inttostr(int32_t a);

void dumptofile(Tree* tree, FILE* file);

void scanstring(string* str);

int32_t mstrs(string* str, char* str2);

int32_t isvalidans(string* str);

void playgame(Tree* tree);

#endif 