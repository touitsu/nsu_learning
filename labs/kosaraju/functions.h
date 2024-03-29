#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <stdint.h>

#ifndef FUNCTIONS_H
#define FUNCTIONS_H

typedef struct vertex {
	int32_t from;
	int32_t to;
}vertex;


uint32_t sumarrnormalized(uint8_t* arr, int32_t n);

void printarr(uint8_t* arr, int32_t n);

void invertmatrix(uint8_t* arr, int32_t n);

void getinp(uint8_t* arr, int32_t n, int32_t m);

void kosaraju(uint8_t* arr, uint8_t* visited, int8_t* res, int32_t n, int32_t m);

#endif