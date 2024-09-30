#pragma once


#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <malloc.h>

#ifndef FUNCTIONS
#define FUNCTIONS

void dijkstra(int32_t n, int32_t cur, int32_t* minplens, int32_t* matrix, int8_t* visited, int32_t* previous, int8_t* ovflcnt);

int32_t shrtpth(int32_t n, int32_t f, int32_t s, int32_t* previous, int32_t* matrix);

int32_t* initarr(int32_t defnum, int32_t n);

#endif