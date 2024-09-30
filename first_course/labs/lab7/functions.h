#pragma once

#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <stdint.h>
#include "functions.h"

typedef struct linkedlist {
	int32_t n;
	struct linkedlist* next;
}linkedlist;

int8_t readinput(int8_t* arr, int32_t n, int32_t m);

void freelinkedlist(linkedlist* ll);

void insert(linkedlist* root, linkedlist* element);

void printlinkedlist(linkedlist* root);

void saveexit(int8_t* arr, linkedlist* res, int8_t* status);

void topsort(int8_t* arr, linkedlist* res, int8_t* status, int32_t node, int32_t n);

#endif