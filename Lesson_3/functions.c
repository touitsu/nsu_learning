#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>


int maximum(int a, int b){
    if (a>b)
        return a;
    return b;
}


int minimum(int a, int b) {
    if (a < b)
        return a;
    return b;
}


char* getinput(){
    char* string = (char*)malloc(sizeof(char) * 2);
    char c = getchar();
    int i = 0;
    while(c != '\n'){
        *(string + i) = c;
        c = getchar();
        i ++;
        string = (char*)realloc(string, sizeof(char) * (i+2));
        if (string == NULL)
            exit(-1);
        
        *(string + i) = '\0';
    }
    return string;
}


int getstrlen(char* string) {
    int len = 0;
    while (*(string + len) != '\0')
        len++;
    
    return len;
}


int getstrarrlen(char** stringarr) {
    int len = 0;
    while (stringarr[len][0] != '\0')
        len++;
    
    return len;
}


char* rvrsstring(char* stringstart, char* stringend) {
    char* newstr = (char*)malloc(sizeof(char) * (stringend - stringstart + 1));
    int i = 0;
    while (stringstart + i  != stringend) {
        *(newstr + i) = *(stringend - i - 1);
        i++;
    }
    *(newstr + i) = '\0';
    return newstr;
}


void printstr(char* string){
    int i = 0;
    while (*(string + i) != '\0'){
        printf("%c", *(string + i));
        i++;
    }
    printf("\n");
}


void printline(char* string) {
    int i = 0;
    while (*(string + i) != '\0') {
        printf("%c", *(string + i));
        i++;
    }
    printf(" ");
}

void printarr(int* arr) {
    for (int i = 0; i < *arr; i++)
        printf("%d\n", *(arr + i));
}

int countmaxspecconrepeats(char* string, char repeat){ //count maximum amount of specified continuous repeats
    int res = 0, curres = 0, i = 0;
    while(*(string + i) != '\0'){
        if (repeat == *(string + i))
            curres ++;
        else{
            res = maximum(res,curres);
            curres = 0;
        }
        i++;
    }
    res = maximum(res,curres);
    return res;
}


int countmaxconrepeats(char* string){
    int res = 0, curres = 0, i = 0, lastc = -1;
    while(*(string + i) != '\0'){
        if(*(string + i) == lastc)
            curres ++;

        else{
            res = maximum(curres,res);
            curres = 1;
            lastc = *(string + i);
        }
        i++;
    }
    res = maximum(curres,res);
    return res;
}


int isinstr(char c, char* string){
    int i = 0;
    while (*(string + i)){
        if(*(string + i) == c)
            return 1;
        i++;
    }
    return 0;
}


int cntunqchrs(char* string, int from, int to){                          // count unique chars
    int res = 0, i = 0;
    char* strofunqchars = (char*)malloc(sizeof(char) * 2);
    while(*(string + i) != '\0'){
        if (*(string + i) >= from && *(string + i) <= to) {
            if (!isinstr(*(string + i), strofunqchars)) {
                *(strofunqchars + res) = *(string + i);
                res++;
                strofunqchars = (char*)realloc(strofunqchars, sizeof(char) * (2 + res));
                if (strofunqchars == NULL)
                    exit(-1);
            }
        }
        i++;
    }
    free(strofunqchars);
    return res;
}


int* cntamunqchrs(char* string) {                                          // counq ammount of unique chars
    int* res = (int*)malloc(sizeof(int) * (127 - ' ' + 1));
    for (int j = 1; j < 127 - ' ' + 1; j++)
        *(res + j) = 0;

    *res = 127 - ' ' + 1;
    int i = 0;
    while (*(string + i) != '\0') {
        *(res + *(string + i) - ' ' + 1) += 1;
        i++;
    }
    return res;
}


int findinarr(int* arr, int value) {
    int res = -1;
    for (int i = 1; i < *arr; i++) {
        if (*(arr + i) == value)
            res = i;
        
    }
    if (res == -1) {
        printf("Couldn't find %d in array", value);
        exit(-1);
    }
    return res;
}


int findinstr(char* stringstart, char* stringend, char element) {
    int i = 0;
    while (*(stringstart + i) != element || stringstart + i != stringend)
        i++;
    
    if (stringstart + i == stringend)
        return -1;

    return i;
}


int* sumarr(int* arr, int* arr1) {
    int* res = (int*)malloc(sizeof(int) * *arr);
    *res = *arr;
    for (int i = 1; i < *res; i++)
        *(res + i) = *(arr + i) + *(arr1 + i);

    return res;
}


char** sprt(char* string, char separator) {
    char** result = (char**)malloc(sizeof(char*));
    char* part = (char*)malloc(sizeof(char) * 2);
    int i = 0, lstsep = 0, j = 0;
    while (*(string + i) != '\0') {
        if (*(string + i) == separator) {
            *(part + i - lstsep) = '\0';
            result = (char**)realloc(result, sizeof(char*) * (j + 1));
            if (result == NULL)
                exit(-1);

            result[j] = part;
            part = (char*)malloc(sizeof(char) * 2);
            j++;
            i++;
            lstsep = i;
        }
        part = (char*)realloc(part, sizeof(char) * (i - lstsep + 2));
        if (part == NULL)
            exit(-1);

        *(part + i - lstsep) = *(string + i);
        i++;
    }
    *(part + i - lstsep) = '\0';
    result = (char**)realloc(result, sizeof(char*) * (j + 1));
    result[j] = part;
    j++;
    part = (char*)malloc(sizeof(char));
    *part = '\0';
    result = (char**)realloc(result, sizeof(char*) * (j + 1));
    if (result == NULL)
        exit(-1);

    result[j] = part;
    return result;
}

void printrvrs(char* string, char separator) {
    char** stringarr = sprt(string, separator);
    for (int i = getstrarrlen(stringarr) - 1; i >= 0; i--) {
        printstr(stringarr[i]);
        free(stringarr[i]);
    }
    free(stringarr[getstrarrlen(stringarr)]);
    free(stringarr);
}


