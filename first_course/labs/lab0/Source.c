#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>


typedef struct string {
	char* content;
	uint16_t length;
}string;


typedef struct number {
	uint64_t integerpart;
	uint16_t floatpoint;
	double floatpart;
}number;


void printstr(string str) {
	
	for (uint16_t i = 0; i < str.length; i++)
		printf("%c", *(str.content + i));

}


void printstrrvrs(string str) {

	for (uint16_t i = str.length; i > 0; i--)
		printf("%c", *(str.content + i - 1));

}


void printarr(uint16_t* arr) {

	for (uint16_t i = 0; i < *arr; i++)
		printf("%hi\n", *(arr + i));

}


void badinput() {
	printf("bad input");
	exit(0);
}


string getinput() {
	string str;
	char* content;
	char c;
	uint16_t i;

	c = getchar();
	while (c == '\n')
		c = getchar();

	content = (char*)malloc(sizeof(char) * 2);

	for (i = 0; c != '\n'; i++) {
		content = (char*)realloc(content, sizeof(char) * (i + 2));
		if (content == NULL)
			exit(-1);

		*(content + i) = c;
		*(content + i + 1) = '\0';

		c = getchar();
	}

	str.content = content;
	str.length = i;

	return str;
}


void toupper(string str) {
	
	for (uint16_t i = 0; i < str.length; i++)
		if (*(str.content + i) >= 'a' && *(str.content + i) <= 'z')
			*(str.content + i) = *(str.content + i) - ('a' - 'A');

}


uint16_t findfloatpoint(string str) {

	for (uint16_t i = 0; i < str.length; i++)
		if (*(str.content + i) == '.')
			return i;

	return UINT16_MAX;
}


number strtonum(string str, uint32_t base) {
	number num;

	num.integerpart = 0;
	num.floatpart = 0.0;

	num.floatpoint = findfloatpoint(str);

	if (num.floatpoint == str.length - 1 || num.floatpoint == 0)
		badinput();

	if (num.floatpoint == UINT16_MAX)
		num.floatpoint = str.length;

	for (uint16_t i = str.length; i >= 1; i--) {

		if (*(str.content + i - 1) >= '0' && *(str.content + i - 1) <= '9') {

			if (i <= num.floatpoint)
				num.integerpart += (uint64_t)((*(str.content + i - 1) - '0') * pow(base, num.floatpoint - i));

			else 
				num.floatpart += (*(str.content + i - 1) - '0') * pow(base, num.floatpoint - i + 1);
			
		}
		else if (*(str.content + i - 1) >= 'A' && *(str.content + i - 1) <= 'F') {

			if (i <= num.floatpoint)
				num.integerpart += (uint64_t)((*(str.content + i - 1) - 'A' + 10) * pow(base, num.floatpoint - i));

			else
				num.floatpart += (*(str.content + i - 1) - 'A' + 10) * pow(base, num.floatpoint - i + 1);

		}
	}

	return num;

}

uint16_t isvalid(string str, uint32_t base) {
	
	int16_t counter;

	counter = 0;

	for (uint16_t i = 0; i < str.length; i++) {
		
		if (*(str.content + i) == '.') {
			counter++;
			if (counter > 1)
				badinput();
		}

		if (*(str.content + i) >= '0' && *(str.content + i) <= '9') {
			if (*(str.content + i) - '0' >= base)
				return 0;
		}

		else if (*(str.content + i) >= 'A' && *(str.content + i) <= 'F') {
			if (*(str.content + i) - 'A' + 10 >= base)
				return 0;
		}

		else if (*(str.content + i) != '.')
			return 0;

	}

	return 1;
}


string* frombase10toother(number num, uint32_t base) {
	string intpart, floatpart;
	string* res;
	uint16_t i;

	intpart.content = (char*)malloc(sizeof(char) * 2);
	i = 0;

	for (i = 0; num.integerpart != 0; i++) {
		intpart.content = (char*)realloc(intpart.content, sizeof(char) * (2 + i));
		if (intpart.content == NULL)
			exit(-1);

		if (num.integerpart % base < 10)
			*(intpart.content + i) = num.integerpart % base + '0';

		if (num.integerpart % base >= 10)
			*(intpart.content + i) = num.integerpart % base + 'A' - 10;

		num.integerpart /= base;
	}

	if (i == 0) {
		i++;
		*intpart.content = '0';
	}

	intpart.length = i;
	*(intpart.content + i) = '\0';

	floatpart.content = (char*)malloc(sizeof(char) * 2);

	for (i = 0; (num.floatpart != 0) && i < 12; i++) {
		floatpart.content = (char*)realloc(floatpart.content, sizeof(char) * (2 + i));
		if (floatpart.content == NULL)
			exit(-1); 

		if ((uint16_t)(num.floatpart * base) < 10)
			*(floatpart.content + i) = (uint16_t)(num.floatpart * base) + '0';

		if ((uint16_t)(num.floatpart * base) >= 10)
			*(floatpart.content + i) = (uint16_t)(num.floatpart * base) + 'A' - 10;

		num.floatpart *= base;
		num.floatpart -= (uint16_t)num.floatpart;
	}

	floatpart.length = i;
	*(floatpart.content + i) = '\0';

	res = (string*)malloc(sizeof(string) * 2);

	*res = intpart;
	*(res + 1) = floatpart;

	return res;
}


int main() {
	uint32_t base1, base2;
	string* res;
	string inp;
	number num;
	
	scanf("%i %i", &base1, &base2);

	inp = getinput();

	toupper(inp);

	num = strtonum(inp, base1);

	if (!isvalid(inp, base1))
		badinput();

	if (base1 < 2 || base1 > 16 || base2 < 2 || base2 > 16)
		badinput();

	res = frombase10toother(num, base2);

	printstrrvrs(*res);
	if (*(res[1].content) != '\0') {
		printf(".");
		printstr(*(res + 1));
	}

	free(res[0].content);
	free(res[1].content);
	free(res);
	free(inp.content);
}
