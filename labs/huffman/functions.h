#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <stdint.h>


#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#pragma pack(push, 1)
typedef struct tree {
	uint32_t val;
	uint64_t fq;
	struct tree* left;
	struct tree* right;
}tree;
#pragma pack(pop)


typedef struct pq {
	tree* val;
	struct pq* next;
}pq;

typedef struct keyvalue {
	uint32_t key;
	char* val;
}kv;


typedef struct string {
	char* content;
	int32_t len;
}string;

pq* initpq();

void insertpq(pq* root, pq* node);

pq* pop(pq** root);

tree* inittree();

void freetree(tree* tr);

int32_t unicodetoint(uint8_t c);

char* inttounicode(uint32_t n);

tree** getleafes(int32_t* n);

tree* huffmantree(tree** leafes, int32_t n);

int32_t strlen(char* str);

char* concat(char* a, char* b);

char* copystr(char* str);

void getpaths(kv* res, tree* tr, char* val);

void reversestring(char* str);

string* huffcodetobytes(char* str);

void dumpkv(FILE* kvout, kv* res, int32_t n);

void encode(FILE* in, FILE* out, FILE* kvout);

void inserttree(tree* tr, tree* node, int32_t n, int32_t curn);

kv* readkv(FILE* in, int32_t* leafescount);

int8_t strcmp(string* str1, string* str2);

int8_t charcmp(char* str1, char* str2);

void moveleft(string* str, FILE* in, int32_t n);

void decode(FILE* in, FILE* out, FILE* kvin);

char* tobin(char* path);

#endif