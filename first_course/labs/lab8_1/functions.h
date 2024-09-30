#pragma once


#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <malloc.h>

#ifndef FUNCTIONS
#define FUNCTIONS

typedef struct vertex {
	int32_t from;
	int32_t to;
	int32_t len;
}vertex;

int8_t readinput(vertex* arr, int32_t n, int32_t m);

void saveexit(vertex* arr, vertex* res);

int32_t arrsum(int8_t* arr, int32_t n);

vertex* prim(vertex* arr, int32_t* reslen, int32_t n, int32_t m);

#endif