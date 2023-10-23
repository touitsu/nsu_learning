#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>


typedef struct string {
	char* content;
	uint16_t len;
}string;


string getinput() {
	string str;
	char c;

	str.content = (char*)malloc(sizeof(char) * 2);
	str.len = 0;

	c = getchar();
	while (c == '\n')
		c = getchar();

	for (uint16_t i = 0; c != '\n'; i++) {
		str.content = (char*)realloc(str.content, sizeof(char) * (i + 2));
		if (str.content == NULL)
			exit(-1);

		*(str.content + i) = c;
		str.len = i + 1;

		c = getchar();
	}

	return str;
}


void printstr(string str) {

	for (uint16_t i = 0; i < str.len; i++)
		printf("%c", *(str.content + i));

	printf("\n");
}


void badinput() {

	printf("bad input");
	exit(0);
}


uint16_t issorted(string str) {

	for (uint16_t i = 0; i < str.len - 1; i++)
		for (uint16_t j = i + 1; j < str.len; j++)
			if (*(str.content + i) < *(str.content + j))
				return 0;

	return 1;
}


uint16_t findbreaking(string str) {

	for (uint16_t i = str.len - 1; i > 1; i--)
		if (*(str.content + i) > *(str.content + i - 1))
			return i - 1;

	return 0;
}


uint16_t findleastbigger(string str, uint16_t idx) {
	uint16_t min;

	min = idx;

	for (uint16_t i = idx + 1; i < str.len; i++) {
		if (min == idx)
			if (*(str.content + min) < *(str.content + i))
				min = i;

		if (*(str.content + min) > *(str.content + i) && *(str.content + i) > *(str.content + idx))
			min = i;

	}

	return min;
}


void swap(string str, uint16_t idx1, uint16_t idx2) {
	char tmp1;

	tmp1 = *(str.content + idx1);
	*(str.content + idx1) = *(str.content + idx2);
	*(str.content + idx2) = tmp1;

	for (uint16_t i = idx1 + 1; i < str.len - 1; i++) {
		for (uint16_t j = i + 1; j < str.len; j++) {
			if (*(str.content + i) > *(str.content + j)) {

				tmp1 = *(str.content + i);
				*(str.content + i) = *(str.content + j);
				*(str.content + j) = tmp1;
			}
		}
	}
}


void combinations(string str, uint16_t n) {
	uint16_t breakidx;

	for (uint16_t i = 0; i < n && !issorted(str); i++) {

		breakidx = findbreaking(str);

		swap(str, breakidx, findleastbigger(str, breakidx));

		printstr(str);
	}
}



void isvalid(string str) {
	uint16_t* counters;

	counters = (uint16_t*)malloc(sizeof(uint16_t) * 10);

	for (uint16_t i = 0; i < 10; i++)
		*(counters + i) = 0;

	for (uint16_t i = 0; i < str.len; i++) {
		if (*(str.content + i) >= '0' && *(str.content + i) <= '9')
			*(counters + *(str.content + i) - '0') += 1;

		else 
			badinput();
	}

	for (uint16_t i = 0; i < 10; i++)
		if (*(counters + i) > 1)
			badinput();
	
	free(counters);
}


int main() {
	string inp;
	uint16_t n;
	
	inp = getinput();

	scanf_s("%hi", &n);

	isvalid(inp);

	combinations(inp, n);

	free(inp.content);

	return 0;
}