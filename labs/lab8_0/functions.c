#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <malloc.h>
#include "functions.h"


int8_t readinput(vertex* arr, int32_t n, int32_t m) {
	char c;
	int32_t from, to, len;

	for (int32_t i = 0; i < m; i++) {
		from = 0;
		to = 0;
		len = 0;
		c = 0;

		while (c == 0 || c == ' ' || c == '\n')
			c = fgetc(stdin);

		while (c != ' ' && c != '\n' && c != EOF) {
			from *= 10;
			from += c - '0';
			c = fgetc(stdin);
		}

		while (c == 0 || c == ' ' || c == '\n')
			c = fgetc(stdin);

		while (c != ' ' && c != '\n' && c != EOF) {
			to *= 10;
			to += c - '0';
			c = fgetc(stdin);
		}

		while (c == 0 || c == ' ' || c == '\n')
			c = fgetc(stdin);

		while (c != ' ' && c != '\n' && c != EOF) {
			if ((int64_t)(len) * 10 > INT32_MAX)
				return 2;

			len *= 10;
			len += c - '0';
			c = fgetc(stdin);
		}

		if (c == EOF && (from == 0 || to == 0))
			return 1;

		if (from < 1 || from > n || to < 1 || to > n)
			return 3;

		(*(arr + i)).from = from;
		(*(arr + i)).to = to;
		(*(arr + i)).len = len;
	}

	return 0;
}


void saveexit(vertex* arr, vertex* res) {
	free(arr);
	if (res)
		free(res);

	fclose(stdin);
	fclose(stdout);
	exit(0);
}

vertex* kruskal(vertex* arr, int32_t* reslen, int32_t n, int32_t m) {
	int32_t min, minidx, tmpmin, tmpmax;
	int8_t* set;
	int8_t setscnt;
	vertex* res;

	res = (vertex*)malloc(sizeof(vertex) * (n - 1));
	set = (int8_t*)malloc(sizeof(int8_t) * n);

	for (int32_t i = 0; i < n; i++)
		*(set + i) = 0;

	*reslen = 0;
	setscnt = 0;

	while (*reslen != n - 1) {
		min = INT32_MAX;
		minidx = -1;

		for (int32_t i = 0; i < m; i++)
			if ((*(arr + i)).len <= min &&
				(*(set + (*(arr + i)).to - 1) != *(set + (*(arr + i)).from - 1)
					|| (*(set + (*(arr + i)).to - 1) == 0 && *(set + (*(arr + i)).from - 1) == 0))
				&& (*(arr + i)).from != (*(arr + i)).to) {
				min = (*(arr + i)).len;
				minidx = i;
			}

		if (minidx == -1) {
			free(set);
			free(res);
			return NULL;
		}

		*(res + (*reslen)++) = (*(arr + minidx));

		if (*(set + (*(arr + minidx)).from - 1) == 0 && *(set + (*(arr + minidx)).to - 1) == 0) {
			*(set + (*(arr + minidx)).from - 1) = setscnt + 1;
			*(set + (*(arr + minidx)).to - 1) = setscnt + 1;
		}
		else if (*(set + (*(arr + minidx)).from - 1) == 0) {
			*(set + (*(arr + minidx)).from - 1) = *(set + (*(arr + minidx)).to - 1);
		}
		else if (*(set + (*(arr + minidx)).to - 1) == 0) {
			*(set + (*(arr + minidx)).to - 1) = *(set + (*(arr + minidx)).from - 1);
		}
		else {
			tmpmin = min(*(set + (*(arr + minidx)).to - 1), *(set + (*(arr + minidx)).from - 1));
			tmpmax = max(*(set + (*(arr + minidx)).to - 1), *(set + (*(arr + minidx)).from - 1));
			for (int32_t i = 0; i < n; i++) {
				if (*(set + i) == tmpmax)
					*(set + i) = tmpmin;
			}
			setscnt--;
		}

		setscnt++;
	}

	free(set);
	return res;
}