#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include "functions.h"


void prntshrtststr(char* string) {
    char** stringarr;
    int stringlen, min;
    
    stringarr = sprt(string, ' ');
    stringlen = getstrarrlen(stringarr);
    min = getstrlen(stringarr[0]);

    for (int i = 0; i < stringlen; i++)
        min = minimum(min, getstrlen(*(stringarr + i)));
    
    printf("%d\n", min);

    freematrix(stringarr);
}


int main() {
    char* str;

    str = getinput();

    prntshrtststr(str);

    free(str);

    return 0;
}
