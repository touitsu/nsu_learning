#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>


int maximum(int a, int b) {
    if (a > b)
        return a;

    return b;
}


int minimum(int a, int b) {
    if (a < b)
        return a;

    return b;
}


char* getinput() {
    char* string;
    char c;

    string = (char*)malloc(sizeof(char) * 2);
    *(string) = '\0';
    c = getchar();

    for (int i = 0; c != '\n'; i++) {
        *(string + i) = c;
        c = getchar();
        string = (char*)realloc(string, sizeof(char) * (i+2));
        if (string == NULL)
            exit(-1);
        
        *(string + i + 1) = '\0';
    }

    return string;
}


int getstrlen(char* string) {
    int i;

    for (i = 0; *(string + i) != '\0'; i++);

    return i;
}


int getstrarrlen(char** stringarr) {
    int i;

    for (i = 0; stringarr[i][0] != '\0'; i++);
    
    return i;
}


char* rvrsstring(char* stringstart, char* stringend) {
    char* newstr;
    int i;

    newstr = (char*)malloc(sizeof(char) * (stringend - stringstart + 1));
    for (i = 0; stringstart + i != stringend; i++)
        *(newstr + i) = *(stringend - i - 1);
    
    *(newstr + i) = '\0';
    return newstr;
}


void printstr(char* string) {
    for (int i = 0; *(string + i) != '\0'; i++)
        printf("%c", *(string + i));
    
    printf("\n");
}


void printline(char* string) {
    for (int i = 0; *(string + i) != '\0'; i++)
        printf("%c", *(string + i));

    printf(" ");
}


void printarr(int* arr) {
    for (int i = 0; i < *arr; i++)
        printf("%d\n", *(arr + i));
}


void printstrarr(char** strarr) {
    int len;

    len = getstrarrlen(strarr);

    for (int i = 0; i < len; i++)
        printstr(strarr[i]);
}


void freematrix(char** stringarr) {
    int len;

    len = getstrarrlen(stringarr);

    for (int i = 0; i < len; i++)
        free(stringarr[i]);
    
    free(stringarr);
}


int countmaxspecconrepeats(char* string, char repeat) { //count maximum amount of specified continuous repeats
    int res = 0, curres = 0;

    res = 0;
    curres = 0;

    for (int i = 0; *(string + i) != '\0'; i++) {
        if (repeat == *(string + i))
            curres ++;

        else {
            res = maximum(res,curres);
            curres = 0;
        }
    }

    res = maximum(res,curres);
    return res;
}


int countmaxconrepeats(char* string) {
    int res, curres, lastc;

    res = 0;
    curres = 0;
    lastc = -1;

    for (int i = 0; *(string + i) != '\0'; i++) {
        if(*(string + i) == lastc)
            curres ++;

        else {

            res = maximum(curres,res);
            curres = 1;
            lastc = *(string + i);
        }
    }

    res = maximum(curres,res);
    return res;
}


int isinstr(char c, char* string) {
    for (int i = 0; *(string + i) != '\0'; i++) {
        if(*(string + i) == c)
            return 1;
    }

    return 0;
}


int cntunqchrs(char* string, int from, int to) {                          // count unique chars
    int res;
    char* strofunqchars;

    res = 0;
    strofunqchars = (char*)malloc(sizeof(char) * 2);

    for (int i = 0; *(string + i) != '\0'; i++) {
        if (*(string + i) >= from && *(string + i) <= to) {
            if (!isinstr(*(string + i), strofunqchars)) {
                *(strofunqchars + res) = *(string + i);
                res++;
                strofunqchars = (char*)realloc(strofunqchars, sizeof(char) * (2 + res));
                if (strofunqchars == NULL)
                    exit(-1);
            }
        }
    }

    free(strofunqchars);
    return res;
}


int* cntamunqchrs(char* string) {                                          // counq ammount of unique chars
    int* res;

    res = (int*)malloc(sizeof(int) * (127 - ' ' + 1));

    for (int j = 1; j < 127 - ' ' + 1; j++)
        *(res + j) = 0;

    *res = 127 - ' ' + 1;

    for(int i = 0; *(string + i) != '\0'; i++)
        *(res + *(string + i) - ' ' + 1) += 1;

    return res;
}


int findinarr(int* arr, int value) {
    int res;

    res = -1;

    for (int i = 1; i < *arr; i++) {
        if (*(arr + i) == value)
            res = i;
        
    }

    if (res == -1)
        printf("Couldn't find %d in array", value);

    return res;
}


int findinstr(char* stringstart, char* stringend, char element) {
    int i;

    i = 0;

    while (*(stringstart + i) != element || stringstart + i != stringend)
        i++;
    
    if (stringstart + i == stringend)
        return -1;

    return i;
}


int* sumarr(int* arr, int* arr1) {
    int* res;

    res = (int*)malloc(sizeof(int) * *arr);
    *res = *arr;

    for (int i = 1; i < *res; i++)
        *(res + i) = *(arr + i) + *(arr1 + i);

    return res;
}


char** sprt(char* string, char separator) {
    char** result;
    char* part;
    int lstsep, j, i;

    result = (char**)malloc(sizeof(char*));
    part = (char*)malloc(sizeof(char) * 2);
    lstsep = 0;
    j = 0;
    for (i = 0; *(string + i) != '\0'; i++) {
        if (*(string + i) == separator) {
            if (!isinstr(separator, part) && getstrlen(part) > 0) {
                j++;

                result = (char**)realloc(result, sizeof(char*) * j);
                if (result == NULL)
                    exit(-1);

                result[j - 1] = part;
                part = (char*)malloc(sizeof(char) * 2);
                *part = '\0';
            }
            lstsep = i + 1;
        }

        else {
            part = (char*)realloc(part, sizeof(char) * (i - lstsep + 2));
            if (part == NULL)
                exit(-1);

            *(part + i - lstsep) = *(string + i);
            *(part + i - lstsep + 1) = '\0';
        }
    }

    j++;
    result = (char**)realloc(result, sizeof(char*) * j);
    if (result == NULL)
        exit(-1);

    *(part + i - lstsep) = '\0';
    result[j - 1] = part;
    j++;
    part = (char*)malloc(sizeof(char));
    *part = '\0';

    result = (char**)realloc(result, sizeof(char*) * j);
    if (result == NULL)
        exit(-1);

    result[j - 1] = part;

    return result;
}


void printrvrs(char** stringarr) {
    int stringarrlen;

    stringarrlen = getstrarrlen(stringarr);

    for (int i = stringarrlen - 1; i >= 0; i--)
        printline(stringarr[i]);

}


void swap(int* a, int* b) {
    int tmp;
    tmp = *a;
    *a = *b;
    *b = tmp;
}

void swapstrs(char** a, char** b) {
    char* tmp;
    
    tmp = *a;
    *a = *b;
    *b = tmp;
}


void sort(int* arr) {
    for (int i = 1; i < *arr; i++) {
        for (int j = 1; j < *arr - i; j++) {
            if (*(arr + j) > *(arr + j + 1))
                swap(arr + j, arr + j + 1);
        }
    }
}


void sortbylen(char** arr) {
    int len;

    len = getstrarrlen(arr);

    for (int i = 0; i < len - 1; i++) {
        for (int j = 0; j < len - i - 1; j++) {
            if (getstrlen(*(arr + j)) > getstrlen(*(arr + j + 1)))
                swapstrs(arr + j, arr + j + 1);
 
        }
    }
}


int comparestr(char* str1, char* str2) {

    if (getstrlen(str1) != getstrlen(str2))
        return 0;

    for (int i = 0; i < getstrlen(str1); i++) {
        if (*(str1 + i) != *(str2 + i))
            return 0;
    }

    return 1;
}


int hasrepeatables(char* string) {
    int* chars;
    int res;

    chars = cntamunqchrs(string);
    res = 0;

    for (int i = 1; i < *chars; i++) {
        if (*(chars + i) > 1)
            res = 1;
    }

    return res;
}


void copy(char* str1, char* str2) {
    int i;

    for (i = 0; *(str2 + i) != '\0'; i++)
        *(str1 + i) = *(str2 + i);

    *(str1 + i) = '\0';
}


char** pythonkeywords() {
    char** keywords;

    keywords = (char**)malloc(sizeof(char*)*34);

    for (int i = 0; i < 34; i++)
        *(keywords + i) = (char*)malloc(sizeof(char) * 9);

    copy(*(keywords), "False\0");
    copy(*(keywords + 1), "None\0");
    copy(*(keywords + 2), "True\0");
    copy(*(keywords + 3), "and\0");
    copy(*(keywords + 4), "as\0");
    copy(*(keywords + 5), "assert\0");
    copy(*(keywords + 6), "break\0");
    copy(*(keywords + 7), "class\0");
    copy(*(keywords + 8), "continue\0");
    copy(*(keywords + 9), "def\0");
    copy(*(keywords + 10), "del\0");
    copy(*(keywords + 11), "elif\0");
    copy(*(keywords + 12), "else\0");
    copy(*(keywords + 13), "except\0");
    copy(*(keywords + 14), "finally\0");
    copy(*(keywords + 15), "for\0");
    copy(*(keywords + 16), "from\0");
    copy(*(keywords + 17), "global\0");
    copy(*(keywords + 18), "if\0");
    copy(*(keywords + 19), "import\0");
    copy(*(keywords + 20), "in\0");
    copy(*(keywords + 21), "is\0");
    copy(*(keywords + 22), "lambda\0");
    copy(*(keywords + 23), "nonlocal\0");
    copy(*(keywords + 24), "not\0");
    copy(*(keywords + 25), "or\0");
    copy(*(keywords + 26), "pass\0");
    copy(*(keywords + 27), "raise\0");
    copy(*(keywords + 28), "return\0");
    copy(*(keywords + 29), "try\0");
    copy(*(keywords + 30), "while\0");
    copy(*(keywords + 31), "with\0");
    copy(*(keywords + 32), "yield\0");
    copy(*(keywords + 33), "\0");

    return keywords;
}


int* fromstrtointarr(char* str) {
    int* arr;

    arr = (int*)malloc(sizeof(int) * (getstrlen(str) + 1));

    *arr = getstrlen(str) + 1;

    for (int i = 0; i < *arr - 1; i++)
        *(arr + i + 1) = *(str + i) - '0';

    return arr;
}