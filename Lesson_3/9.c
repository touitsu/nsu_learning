#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include "functions.h"


int main() {
    char* str;
    char** strarr;
    int len;

    str = getinput();
    strarr = sprt(str, ' ');
    
    len = getstrarrlen(strarr);

    for (int i = 0; i < len - 1; i++) {
        for (int j = i + 1; j < len; j++) {
            if (comparestr(*(strarr + i), *(strarr + j))) {
                printline(*(strarr + i));
                printline(*(strarr + j));
                i++;
                printf("\n");
            }
        }
    }

    freematrix(strarr);
    free(str);
    
    return 0;
}
