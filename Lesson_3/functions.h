#ifndef FUNCTIONS_H_INCLUDED
#define FUNCTIONS_H_INCLUDED

int maximum(int a, int b);

int minimum(int a, int b);

char* getinput();

int getstrlen(char* string);

int getstrarrlen(char** stringarr);

char* rvrsstring(char* stringstart, char* stringend);

void printstr(char* string);

void printline(char* string);

void printarr(int* arr);

int countmaxspecconrepeats(char* string, char repeat);

int countmaxconrepeats(char* string);

int isinstr(char c, char* string);

int cntunqchrs(char* string, int from, int to);

int* cntamunqchrs(char* string);

int findinarr(int* arr, int value);

int findinstr(char* stringstart, char* stringend, char element);

int* sumarr(int* arr, int* arr1);

char** sprt(char* string, char separator);

void printrvrs(char* string, char separator);

#endif // FUNCTIONS_H_INCLUDED
